#pragma once

#include <chrono>
#include <vsg/all.h>

class VSG_DECLSPEC imguiNode : public vsg::Inherit<vsg::StateGroup, imguiNode>
{
public:
    imguiNode(vsg::ref_ptr<vsg::Window> window);

    bool init();

    void render();

protected:
    virtual ~imguiNode() override;

private:
    vsg::ref_ptr<vsg::Window> _window;
    vsg::ref_ptr<vsg::DescriptorSet> _descriptorSet;
    vsg::ImageInfo _imageInfo;

    // bool _initialised;
    std::chrono::time_point<std::chrono::system_clock> _time;
};

namespace vsg
{
    VSG_type_name(imguiNode);
}
