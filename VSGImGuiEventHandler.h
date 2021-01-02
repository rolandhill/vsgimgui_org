#ifndef VSG_IMGUI_EVENT_HANDLER_DEF
#define VSG_IMGUI_EVENT_HANDLER_DEF

#include <vsg/all.h>
#include <chrono>

class VSG_DECLSPEC VSGImGuiEventHandler: public vsg::Inherit<vsg::Visitor, VSGImGuiEventHandler>
{
    public:
        VSGImGuiEventHandler();
        ~VSGImGuiEventHandler();

        void apply(vsg::ButtonPressEvent& buttonPress) override;
        void apply(vsg::ButtonReleaseEvent& buttonRelease) override;
        void apply(vsg::MoveEvent& moveEvent) override;
        void apply(vsg::ScrollWheelEvent& scrollWheel) override;
        void apply(vsg::ConfigureWindowEvent& configureWindow) override;
        void apply(vsg::FrameEvent& frame) override;

    protected:
        bool _dragging;
        
    private:
        uint32_t _convertButton( uint32_t button );
        std::chrono::high_resolution_clock::time_point t0;
};

namespace vsg
{
    VSG_type_name(VSGImGuiEventHandler);
}

#endif
