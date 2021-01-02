#include "VSGImGuiEventHandler.h"
#include "imgui.h"

VSGImGuiEventHandler::VSGImGuiEventHandler() :
    _dragging(false)
{
    t0 = std::chrono::high_resolution_clock::now();
}

VSGImGuiEventHandler::~VSGImGuiEventHandler()
{}

void VSGImGuiEventHandler::apply(vsg::ButtonPressEvent& buttonPress)
{
    ImGuiIO &io = ImGui::GetIO();

    if( io.WantCaptureMouse )
    {
        uint32_t button = _convertButton( buttonPress.button );
        io.MouseDown[button] = true;
        io.MousePos.x = buttonPress.x;
        io.MousePos.y = buttonPress.y;

        buttonPress.handled = true;
    }
    else
    {
        _dragging = true;
    }

}

void VSGImGuiEventHandler::apply(vsg::ButtonReleaseEvent& buttonRelease)
{
    ImGuiIO &io = ImGui::GetIO();
    if( (!_dragging) && io.WantCaptureMouse )
    {
        uint32_t button = _convertButton( buttonRelease.button );
        io.MouseDown[button] = false;
        io.MousePos.x = buttonRelease.x;
        io.MousePos.y = buttonRelease.y;

        buttonRelease.handled = true;
    }

    _dragging = false;
}

void VSGImGuiEventHandler::apply(vsg::MoveEvent& moveEvent)
{
    if ( !_dragging )
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MousePos.x = moveEvent.x;
        io.MousePos.y = moveEvent.y;

        moveEvent.handled =  io.WantCaptureMouse;
    }
}

void VSGImGuiEventHandler::apply(vsg::ScrollWheelEvent& scrollWheel)
{
    if ( !_dragging )
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseWheel += scrollWheel.delta[1];
        scrollWheel.handled =  io.WantCaptureMouse;
    }
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

    auto t1 = std::chrono::high_resolution_clock::now();
    double dt = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count();
    t0 = t1;

    io.DeltaTime = dt;
}


uint32_t VSGImGuiEventHandler::_convertButton( uint32_t button )
{
    return button == 1 ? 0 : button == 3 ? 1 : button;
}

