#ifndef VSG_IMGUI_EVENT_HANDLER_DEF
#define VSG_IMGUI_EVENT_HANDLER_DEF

#include <vsg/all.h>

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

    private:
        uint32_t _convertButton( uint32_t button );
};

namespace vsg
{
    VSG_type_name(VSGImGuiEventHandler);
}

#endif
