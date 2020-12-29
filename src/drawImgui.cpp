#include "drawImgui.h"
#include "imguiNode.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_vulkan.h"

drawImgui::drawImgui(vsg::Device* device, vsg::ref_ptr<vsg::DescriptorSet> descriptorSet, vsg::ImageInfo& imageInfo) :
    _device(device),
    _descriptorSet(descriptorSet),
    _imageInfo(imageInfo)
{

}

void drawImgui::record(vsg::CommandBuffer& commandBuffer) const
{
    // Record dear imgui primitives into command buffer
    ImDrawData* draw_data = ImGui::GetDrawData();

    ImGui_ImplVulkan_RenderDrawData(draw_data, &(*commandBuffer));

    // VkDescriptorImageInfo desc_image[1] = {};
    // desc_image[0].sampler = _imageInfo.sampler->vk(_device->deviceID);
    // desc_image[0].imageView = _imageInfo.imageView->vk(_device->deviceID);
    // desc_image[0].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    // VkWriteDescriptorSet write_desc[1] = {};
    // write_desc[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    // write_desc[0].dstSet = _descriptorSet->vk(_device->deviceID);
    // write_desc[0].descriptorCount = 1;
    // write_desc[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    // write_desc[0].pImageInfo = desc_image;
    // vkUpdateDescriptorSets(_device->getDevice(), 1, write_desc, 0, NULL);
}

