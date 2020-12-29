#include <vsg/all.h>

#ifdef USE_VSGXCHANGE
#include <vsgXchange/ReaderWriter_all.h>
#endif

#include <iostream>

#include "imguiNode.h"

vsg::ref_ptr<vsg::Camera> createCameraForScene(vsg::Node* scenegraph, int32_t x, int32_t y, uint32_t width, uint32_t height)
{
    // compute the bounds of the scene graph to help position camera
    vsg::ComputeBounds computeBounds;
    scenegraph->accept(computeBounds);
    vsg::dvec3 centre = (computeBounds.bounds.min+computeBounds.bounds.max)*0.5;
    double radius = vsg::length(computeBounds.bounds.max-computeBounds.bounds.min)*0.6;
    double nearFarRatio = 0.001;

    // set up the camera
    auto lookAt = vsg::LookAt::create(centre+vsg::dvec3(0.0, -radius*3.5, 0.0),
                                      centre, vsg::dvec3(0.0, 0.0, 1.0));

    auto perspective = vsg::Perspective::create(30.0, static_cast<double>(width) / static_cast<double>(height),
                                                nearFarRatio*radius, radius * 4.5);

    auto viewportstate = vsg::ViewportState::create(x, y, width, height);

    return vsg::Camera::create(perspective, lookAt, viewportstate);
}

vsg::ref_ptr<vsg::Camera> createOrthoCameraForScene(vsg::Node* vsg_scene, vsg::ref_ptr<vsg::Window> window)
{
    // compute the bounds of the scene graph to help position camera
    vsg::ComputeBounds computeBounds;
    vsg_scene->accept(computeBounds);
    vsg::dvec3 centre = (computeBounds.bounds.min+computeBounds.bounds.max)*0.5;
    double radius = vsg::length(computeBounds.bounds.max-computeBounds.bounds.min)*0.6;
    double nearFarRatio = 0.001;


    // set up the camera
    auto lookAt = vsg::LookAt::create(centre+vsg::dvec3(0.0, -radius*3.5, 0.0), centre, vsg::dvec3(0.0, 0.0, 1.0));

    double aspectRatio = static_cast<double>(window->extent2D().width) / static_cast<double>(window->extent2D().height);
    double halfDim = radius * 1.1;
    double halfHeight, halfWidth;
    if (window->extent2D().width > window->extent2D().height)
    {
        halfHeight = halfDim;
        halfWidth = halfDim * aspectRatio;
    }
    else
    {
        halfWidth = halfDim;
        halfHeight = halfDim / aspectRatio;
    }
    auto projection = vsg::Orthographic::create(-halfWidth, halfWidth,
                                                -halfHeight, halfHeight,
                                                nearFarRatio*radius, radius * 4.5);

    auto camera = vsg::Camera::create(projection, lookAt, vsg::ViewportState::create(window->extent2D()));

    return camera;
}

vsg::ref_ptr<vsg::Camera> createCameraForHud(uint32_t hudwidth, uint32_t hudheight, uint32_t vwidth, uint32_t vheight)
{
    // double aspectRatio = static_cast<double>(vwidth) / static_cast<double>(vheight);
    // width = height * aspectRatio;
    vsg::dvec3 centre = vsg::dvec3(hudwidth * 0.5, hudheight * 0.5, 0.0);
    // vsg::dvec3 centre = vsg::dvec3(width * 0.5, height * 0.5, 0.0);
    // double radius = vsg::length(computeBounds.bounds.max-computeBounds.bounds.min)*0.6;
    // double nearFarRatio = 0.001;

    // set up the camera
    auto lookAt = vsg::LookAt::create(centre+vsg::dvec3(0.0, 4.0, 4.0),
                                      centre, vsg::dvec3(0.0, 1.0, 0.0));

    // auto projection = vsg::Perspective::create(30.0, static_cast<double>(5) / static_cast<double>(5),
                                                // 0.001*5, 5 * 4.5);
    auto projection = vsg::Orthographic::create(0, hudwidth, 0, hudheight, 0.1, 10.0);
    // auto projection = vsg::Orthographic::create(width * -0.5, width * 0.5, height * -0.5, height * 0.5, 0.1, 10.0);

    auto camera = vsg::Camera::create(projection, lookAt, vsg::ViewportState::create(VkExtent2D{vwidth, vheight}));

    return camera;
}


int main(int argc, char** argv)
{
    // set up defaults and read command line arguments to override them
    auto options = vsg::Options::create();
    
    auto windowTraits = vsg::WindowTraits::create();
    windowTraits->windowTitle = "SI IMGUI";

    // set up defaults and read command line arguments to override them
    vsg::CommandLine arguments(&argc, argv);
    windowTraits->debugLayer = arguments.read({"--debug","-d"});
    windowTraits->apiDumpLayer = arguments.read({"--api","-a"});
    if (arguments.read({"--fullscreen", "--fs"})) windowTraits->fullscreen = true;
    if (arguments.read({"--window", "-w"}, windowTraits->width, windowTraits->height)) { windowTraits->fullscreen = false; }
    auto horizonMountainHeight = arguments.value(0.0, "--hmh");
    arguments.read("--screen", windowTraits->screenNum);
    arguments.read("--display", windowTraits->display);

    if (arguments.errors()) return arguments.writeErrorMessages(std::cerr);

#ifdef USE_VSGXCHANGE
    // add use of vsgXchange's support for reading and writing 3rd party file formats
    options->readerWriter = vsgXchange::ReaderWriter_all::create();
#endif

    std::vector<vsg::ref_ptr<vsg::Node>> nodes;

    // read any vsg files from command line arguments
    for (int i=1; i<argc; ++i)
    {
        vsg::Path filename = arguments[i];

        auto loaded_scene = vsg::read_cast<vsg::Node>(filename, options);
        if (loaded_scene)
        {
            nodes.push_back(loaded_scene);
            arguments.remove(i, 1);
            --i;
        }
    }

    // assign the scene from the loaded nodes
    vsg::ref_ptr<vsg::Node> scene;
    if (nodes.size()>1) scene = vsg::Group::create(nodes.begin(), nodes.end());
    else if (nodes.size()==1) scene = nodes.front();

    if (!scene)
    {
        std::cout<<"No scene loaded, please specify 3d model on command line."<<std::endl;
        return 1;
    }

    // create the viewer and assign window(s) to it
    auto viewer = vsg::Viewer::create();

    auto window = vsg::Window::create(windowTraits);
    if (!window)
    {
        std::cout<<"Could not create windows."<<std::endl;
        return 1;
    }

    viewer->addWindow(window);

    uint32_t width = window->extent2D().width;
    uint32_t height = window->extent2D().height;

    auto renderGraph = vsg::RenderGraph::create(window);

    // create view1
    auto camera = createCameraForScene(scene, 0, 0, width, height);
    auto view1 = vsg::View::create(camera, scene);
    renderGraph->addChild(view1);

    // clear the depth buffer before view2 gets rendered
    VkClearAttachment attachment{VK_IMAGE_ASPECT_DEPTH_BIT, 1, VkClearValue{1.0f, 0.0f}};
    VkClearRect rect{VkRect2D{VkOffset2D{0, 0}, VkExtent2D{width, height}}, 0, 1};
    auto clearAttachments = vsg::ClearAttachments::create(vsg::ClearAttachments::Attachments{attachment}, vsg::ClearAttachments::Rects{rect});
    renderGraph->addChild(clearAttachments);


    
    // add close handler to respond to pressing the window close window button and pressing escape
    viewer->addEventHandler(vsg::CloseHandler::create(viewer));

    // add a trackball event handler to control the camera view use the mouse
    viewer->addEventHandler(vsg::Trackball::create(camera));

    // create a command graph to render the scene on specified window
    // auto commandGraph = vsg::createCommandGraphForView(window, camera, scene);
    auto commandGraph = vsg::CommandGraph::create(window);

    commandGraph->addChild(renderGraph);

    viewer->assignRecordAndSubmitTaskAndPresentation({commandGraph});

    auto hud = imguiNode::create(window);
    // create view2
    // auto secondary_camera = createCameraForScene(scene, 0, 0, width, height);
    auto secondary_camera = createCameraForHud(width, height, width, height);
    // auto secondary_camera = createOrthoCameraForScene(scene, window);
    // auto secondary_camera = createCameraForHud(5, 5);
    auto view2 = vsg::View::create(secondary_camera, hud);
    // auto view2 = vsg::View::create(secondary_camera, scene);
    renderGraph->addChild(view2);

    // viewer->assignRecordAndSubmitTaskAndPresentation({commandGraph});

    // compile all the the Vulkan objects and transfer data required to render the scene
    viewer->compile();

    bool hudinit = false;

    // rendering main loop
    while (viewer->advanceToNextFrame())
    {

        // pass any events into EventHandlers assigned to the Viewer
        viewer->handleEvents();
        if (hudinit) hud->render();

        viewer->update();

        viewer->recordAndSubmit();

        viewer->present();

        if(!hudinit)
        {
            hud->init();
            hudinit = true;
        }

    }

    // clean up done automatically thanks to ref_ptr<>
    return 0;
}
