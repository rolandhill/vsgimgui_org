#include "VSGImGuiEventHandler.h"
#include "imgui.h"

VSGImGuiEventHandler::VSGImGuiEventHandler()
{}

VSGImGuiEventHandler::~VSGImGuiEventHandler()
{}

void VSGImGuiEventHandler::apply(vsg::ButtonPressEvent& buttonPress)
{
    uint32_t button = _convertButton( buttonPress.button );
    ImGuiIO &io = ImGui::GetIO();
    io.MouseDown[button] = true;
    io.MousePos.x = buttonPress.x;
    io.MousePos.y = buttonPress.y;
}

void VSGImGuiEventHandler::apply(vsg::ButtonReleaseEvent& buttonRelease)
{
    uint32_t button = _convertButton( buttonRelease.button );
    ImGuiIO &io = ImGui::GetIO();
    io.MouseDown[button] = false;
    io.MousePos.x = buttonRelease.x;
    io.MousePos.y = buttonRelease.y;
}

void VSGImGuiEventHandler::apply(vsg::MoveEvent& moveEvent)
{
    ImGuiIO &io = ImGui::GetIO();
    io.MousePos.x = moveEvent.x;
    io.MousePos.y = moveEvent.y;
}

void VSGImGuiEventHandler::apply(vsg::ScrollWheelEvent& scrollWheel)
{
    ImGuiIO &io = ImGui::GetIO();
    io.MouseWheel += scrollWheel.delta[1];
}

void VSGImGuiEventHandler::apply(vsg::ConfigureWindowEvent& configureWindow)
{
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize.x = configureWindow.width;
    io.DisplaySize.y = configureWindow.height;
}

void VSGImGuiEventHandler::apply(vsg::FrameEvent& frame)
{
    ImGuiIO &io = ImGui::GetIO();

    // Should figure this out for real
    io.DeltaTime = 1.0f/60.f;
}


uint32_t VSGImGuiEventHandler::_convertButton( uint32_t button )
{
    return button == 1 ? 0 : button == 3 ? 1 : button;
}

