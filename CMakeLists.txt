cmake_minimum_required(VERSION 3.7)

project(sigui
        VERSION 0.0.0
        DESCRIPTION "GUI tests for SI"
        LANGUAGES CXX
)

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
    src/si.cpp
    src/imguiNode.cpp
    src/drawImgui.cpp
    external/imgui/imgui.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_impl_vulkan.cpp
)

add_executable(sigui ${SOURCES})

target_include_directories( sigui PUBLIC
    include
    external
)

target_link_libraries(sigui vsg::vsg)

if (vsgXchange_FOUND)
    target_compile_definitions(sigui PRIVATE USE_VSGXCHANGE)
    target_link_libraries(sigui vsgXchange::vsgXchange)
endif()

install(TARGETS sigui
        RUNTIME DESTINATION bin
)

