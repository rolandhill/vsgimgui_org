#ifndef VSG_IMGUI_EVENT_HANDLER_DEF
#define VSG_IMGUI_EVENT_HANDLER_DEF

#include <vsg/all.h>
#include <chrono>

#include "VSGImGui.h"

class VSG_DECLSPEC VSGImGuiEventHandler: public vsg::Inherit<vsg::Visitor, VSGImGuiEventHandler>
{
    public:
        VSGImGuiEventHandler(vsg::ref_ptr<vsgImGui> vsgImgui);
        ~VSGImGuiEventHandler();

        void apply(vsg::ButtonPressEvent& buttonPress) override;
        void apply(vsg::ButtonReleaseEvent& buttonRelease) override;
        void apply(vsg::MoveEvent& moveEvent) override;
        void apply(vsg::ScrollWheelEvent& scrollWheel) override;
        void apply(vsg::KeyPressEvent& keyPress) override;
        void apply(vsg::KeyReleaseEvent& keyRelease) override;
        void apply(vsg::ConfigureWindowEvent& configureWindow) override;
        void apply(vsg::FrameEvent& frame) override;

    private:
        uint32_t _convertButton( uint32_t button );
        std::chrono::high_resolution_clock::time_point t0;

        vsg::ref_ptr<vsgImGui> _vsgImgui;

    protected:
        bool _dragging;

        void initKeymap();
};

namespace vsg
{
    VSG_type_name(VSGImGuiEventHandler);
}

#endif
