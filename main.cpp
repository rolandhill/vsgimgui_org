#include "VSGImGui.h"
#include "VSGImGuiEventHandler.h"
#include "imgui/imgui.h"

#include <vsg/all.h>

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
            viewer->handleEvents();

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

    return 0;
}
