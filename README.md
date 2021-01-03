# vsgimgui
Integrating ImGui with VulkanSceneGraph  
2021  
Roland Hill  
Don Burns  

This repository contains files for integrating [Dear ImGui](https://github.com/ocornut/imgui) with [Vulkan Scene Graph](https://github.com/vsg-dev/VulkanSceneGraph). To use, follow these steps:

1. Set up a RenderGraph so that the Dear ImGui user interface can be rendered on top of your 3D scene.
1. Create an instance of vsgImGui and add it to the renderGraph.
1. Create your own callback class with the ImGui widgets you want to render in a () function and register it with vsgImGui using setRenderCallback(). You can use MyRenderCallback in the example (main.cpp) as a template.
1. Create an instance of vsgImGuiEventHandler and add it as a handler to the viewer, passing in a reference to the vsgImGui instance above. Note that it should be added first so that ImGui can grab any mouse and keyboard input it needs then pass other events to the rest of VSG.

There is a example program to demostrate use (main.cpp). To build it:

1. cmake .
1. make
1. cd bin
1. ./vsgimgui

The teapot.vsgt file should have been copied into the bin directory automatically. If not, copy it manually.

The Dear ImGui implementation used here was current as of January 2021. If you want to update it then simply copy over the files in the imgui directory in the example, or use updated files in your own project.

The source code is published under the [MIT License](LICENSE.md).

