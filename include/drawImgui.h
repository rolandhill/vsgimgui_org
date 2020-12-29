#pragma once

#include <vsg/all.h>

class imguiNode;

class VSG_DECLSPEC drawImgui : public vsg::Inherit<vsg::Command, drawImgui>
{
public:
    drawImgui(vsg::Device* device, vsg::ref_ptr<vsg::DescriptorSet> descriptorSet, vsg::ImageInfo& imageInfo );

    void record(vsg::CommandBuffer& commandBuffer) const override;

protected:
    // vsg::ref_ptr<imguiNode> _imguiNode;
    vsg::Device* _device;
    vsg::ref_ptr<vsg::DescriptorSet> _descriptorSet;
    vsg::ImageInfo& _imageInfo;

private:
};

namespace vsg
{
    VSG_type_name(drawImgui);
}
