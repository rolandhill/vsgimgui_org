#ifndef VSG_IMGUI_DEF
#define VSG_IMGUI_DEF
#include <vsg/viewer/Window.h>

class VSG_DECLSPEC vsgImGui: public vsg::Inherit<vsg::Command, vsgImGui>
{
    public:
        vsgImGui( const vsg::ref_ptr<vsg::Window> &window );
        void setShowDemoWindow( bool flag);
        bool getShowDemoWindow() const;

        void render();

        void record(vsg::CommandBuffer& commandBuffer) const override;

    private:
        bool _showDemoWindow;
        VkInstance _instance;
        VkDevice _device;
        uint32_t _queueFamily;
        VkQueue _queue;

        void _init( const vsg::ref_ptr<vsg::Window> &window );
        void _uploadFonts( const vsg::ref_ptr<vsg::Window> &window );
};

namespace vsg
{
    VSG_type_name(vsgImGui);
}

#endif
