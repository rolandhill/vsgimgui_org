# vsgimgui
Integrating ImGui with VulkanSceneGraph  
2021  
Roland Hill  
Don Burns  

This repository contains files for integrating Dear ImGui with VSG. To use, follow these steps:

1. Set up a RenderGraph so that the Dear ImGui user interface can be rendered on top of your 3D scene.  
1. Create an instance of vsgImGui and add it to the renderGraph.  
1. Create an instance of VSGImGuiEventHandler and add it as a handler to the viewer. Note that it should be added first so that ImGui can grab any mouse and keyboard input it needs then pass other events to the rest of VSG.

There is a example program to demostrate use. If you want to build and run the example you will need to copy teapot.vsgt into your bin directory as it's hardwired.

The source code is published under the [MIT License](LICENSE.md).

