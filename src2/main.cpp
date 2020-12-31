#include "VSGImGui.h"
#include "VSGImGuiEventHandler.h"

#include "imgui.h"
#include <vsg/all.h>

#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>

#if 0
#ifndef VSG_IMGUI_EVENT_HANDLER_DEF
#define VSG_IMGUI_EVENT_HANDLER_DEF

class VSGImGuiEventHandler: public vsg::Inherit<vsg::Visitor, VSGImGuiEventHandler>
{
    public:
        VSGImGuiEventHandler()
        {}

        ~VSGImGuiEventHandler()
        {}

        void apply(vsg::ButtonPressEvent& buttonPress) override
        {
            uint32_t button = _convertButton( buttonPress.button );
            ImGuiIO &io = ImGui::GetIO();
            io.MouseDown[button] = true;
            io.MousePos.x = buttonPress.x;
            io.MousePos.y = buttonPress.y;
        }

        void apply(vsg::ButtonReleaseEvent& buttonRelease) override
        {
            uint32_t button = _convertButton( buttonRelease.button );
            ImGuiIO &io = ImGui::GetIO();
            io.MouseDown[button] = false;
            io.MousePos.x = buttonRelease.x;
            io.MousePos.y = buttonRelease.y;
        }

        void apply(vsg::MoveEvent& moveEvent) override
        {
            ImGuiIO &io = ImGui::GetIO();
            io.MousePos.x = moveEvent.x;
            io.MousePos.y = moveEvent.y;
        }

        void apply(vsg::ScrollWheelEvent& scrollWheel) override
        {
            ImGuiIO &io = ImGui::GetIO();
            io.MouseWheel += scrollWheel.delta[1];
        }

        void apply(vsg::ConfigureWindowEvent& configureWindow) override
        {
            ImGuiIO &io = ImGui::GetIO();
            io.DisplaySize.x = configureWindow.width;
            io.DisplaySize.y = configureWindow.height;
        }

        void apply(vsg::FrameEvent& frame) override
        {
            ImGuiIO &io = ImGui::GetIO();

            // Should figure this out for real
            io.DeltaTime = 1.0f/60.f;
        }


    private:
        uint32_t _convertButton( uint32_t button )
        {
            return button == 1 ? 0 : button == 3 ? 1 : button;
        }
};


namespace vsg
{
    VSG_type_name(VSGImGuiEventHandler);
}

#endif
#endif

int main(int argc, char** argv)
{
    // set up defaults and read command line arguments to override them
    auto options = vsg::Options::create();

    auto windowTraits = vsg::WindowTraits::create();
    windowTraits->windowTitle = "vsgviewer";

    // set up defaults and read command line arguments to override them
    vsg::CommandLine arguments(&argc, argv);
    arguments.read(options);

    windowTraits->debugLayer = arguments.read({"--debug","-d"});
    windowTraits->apiDumpLayer = arguments.read({"--api","-a"});

    // enable required device features.
    windowTraits->deviceFeatures.samplerAnisotropy = VK_TRUE;

    if (arguments.errors()) return arguments.writeErrorMessages(std::cerr);

    try 
    {
        auto vsg_scene = vsg::Group::create();

        if (auto node = vsg::read_cast<vsg::Node>("teapot.vsgt"); node) vsg_scene->addChild(node);


        // create the viewer and assign window(s) to it
        auto viewer = vsg::Viewer::create();

        vsg::ref_ptr<vsg::Window> window(vsg::Window::create(windowTraits));
        if (!window)
        {
            std::cout<<"Could not create windows."<<std::endl;
            return 1;
        }

        viewer->addWindow(window);

        // compute the bounds of the scene graph to help position camera
        vsg::ComputeBounds computeBounds;
        vsg_scene->accept(computeBounds);
        vsg::dvec3 centre = (computeBounds.bounds.min+computeBounds.bounds.max)*0.5;
        double radius = vsg::length(computeBounds.bounds.max-computeBounds.bounds.min)*0.6;

        // These are set statically because the geometry in the class is expanded in the shader
        double nearFarRatio = 0.01;

        // set up the camera
        auto lookAt = vsg::LookAt::create(centre+vsg::dvec3(0.0, -radius*3.5, 0.0), centre, vsg::dvec3(0.0, 0.0, 1.0));

        vsg::ref_ptr<vsg::ProjectionMatrix> perspective;
        if (vsg::ref_ptr<vsg::EllipsoidModel> ellipsoidModel(vsg_scene->getObject<vsg::EllipsoidModel>("EllipsoidModel")); ellipsoidModel)
        {
            perspective = vsg::EllipsoidPerspective::create(lookAt, ellipsoidModel, 30.0, static_cast<double>(window->extent2D().width) / static_cast<double>(window->extent2D().height), nearFarRatio, 0.0);
        }
        else
        {
            perspective = vsg::Perspective::create(30.0, static_cast<double>(window->extent2D().width) / static_cast<double>(window->extent2D().height), nearFarRatio*radius, radius * 400.5);
        }

        auto camera = vsg::Camera::create(perspective, lookAt, vsg::ViewportState::create(window->extent2D()));

        // add close handler to respond the close window button and pressing escape
        viewer->addEventHandler(vsg::CloseHandler::create(viewer));

        viewer->addEventHandler(vsg::Trackball::create(camera));


        auto commandGraph = vsg::createCommandGraphForView(window, camera, vsg_scene);

        ///////////////////////////////////////////////////////////

        viewer->addEventHandler(VSGImGuiEventHandler::create());

        auto gui = vsgImGui::create(window);
        commandGraph->addChild(gui);

        ///////////////////////////////////////////////////////////

        viewer->assignRecordAndSubmitTaskAndPresentation({commandGraph});

        viewer->compile();

        // rendering main loop
        while (viewer->advanceToNextFrame() )
        {
            // pass any events into EventHandlers assigned to the Viewer
            //vsg::UIEvents events = viewer->getEvents();
            //std::cout << events.size() << std::endl;
            viewer->handleEvents();

            /*
            {
                ImGuiIO &io = ImGui::GetIO();

                io.DeltaTime = 1.0f/60.f;
                io.DisplaySize.x = window->extent2D().width;
                io.DisplaySize.y = window->extent2D().height;
            }
            */

            gui->render();

            viewer->update();

            viewer->recordAndSubmit();

            viewer->present();
        }
    }
    catch( vsg::Exception &ve )
    {
        std::cerr << "[Exception] - " << ve.message << std::endl;
    }

    // clean up done automatically thanks to ref_ptr<>
    return 0;
}
