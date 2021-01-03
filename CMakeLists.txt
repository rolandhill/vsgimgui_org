cmake_minimum_required(VERSION 3.7)

project(vsgimgui)

# build all examples into the bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Change the default build type to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# find the vsg
if (VULKAN_SDK)
    set(ENV{VULKAN_SDK} ${VULKAN_SDK})
endif()

find_package(vsg REQUIRED)

# find the optional vsgXchange that can be used for reading and range of image and 3d model formats and shader compilation
find_package(vsgXchange QUIET)

# set the use of C++17 globally as all examples require it
set(CMAKE_CXX_STANDARD 17)

# add clobber build target to clear all the non git registered files/directories
add_custom_target(clobber
    COMMAND git clean -d -f -x
)

set(SOURCES
        main.cpp
        VSGImGui.cpp
        VSGImGuiEventHandler.cpp

        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_impl_vulkan.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
)

add_executable(vsgimgui ${SOURCES})

target_include_directories( vsgimgui PUBLIC
    imgui
)

target_link_libraries(vsgimgui vsg::vsg )

install( TARGETS vsgimgui
        RUNTIME DESTINATION bin
)

configure_file(teapot.vsgt ${PROJECT_BINARY_DIR}/bin/teapot.vsgt COPYONLY)

