#pragma once

/*
Copyright(c) 2021 Roland Hill

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom 
the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH 
THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <vsg/viewer/Window.h>
#include <functional>

class VSG_DECLSPEC vsgImGui: public vsg::Inherit<vsg::Command, vsgImGui>
{
    public:
        vsgImGui( const vsg::ref_ptr<vsg::Window> &window );
        virtual ~vsgImGui();

        using RenderCallback = std::function<void()>;
        void setRenderCallback(const RenderCallback &callback);

        void render() const;

        void record(vsg::CommandBuffer& commandBuffer) const override;

        void setShowDemoWindow(bool);
        bool getShowDemoWindow() const;

    private:
        VkInstance _instance;
        VkDevice _device;
        uint32_t _queueFamily;
        VkQueue _queue;
        VkDescriptorPool _descriptorPool;
        VkCommandPool _commandPool;
        RenderCallback _renderCallback;
        bool           _showDemoWindow = false;


        void _init( const vsg::ref_ptr<vsg::Window> &window );
        void _uploadFonts( const vsg::ref_ptr<vsg::Window> &window );
};

namespace vsg
{
    VSG_type_name(vsgImGui);
}
