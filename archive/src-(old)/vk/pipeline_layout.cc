#include "xgpu/vk/pipeline_layout.h"
#include "xgpu/vk/device.h"
#include "xgpu/core/log.h"
#include "xgpu/vk/descriptor_set.h"
#include "xgpu/vk/pipeline.h"
#include "vk/vk_utils.h"
#include <vulkan/vulkan_core.h>

namespace xgpu::vk
{
    auto PipelineLayout::from_open(xgpu::Device &device, const xgpu::PipelineLayoutDescription &description) noexcept -> expected<vk::PipelineLayout, Error>
    {
        auto* vk_device = dynamic_cast<vk::Device*>(device.handle());
        if (!vk_device)
        {
            return unexpected( Error("Failed to get vulkan device from xgpu::Device") );
        }
        vk::PipelineLayout pipeline_layout { vk_device->handle() };

        auto push_constants = [&] {
            std::vector<VkPushConstantRange> push_constants {};
            for (const auto& push_constant : description.push_constants)
            {
                push_constants.push_back(getVulkanPushConstantRange(push_constant));
            }

            return push_constants;
        }();

        auto expected_descriptor_sets = [&]() -> expected<std::vector<VkDescriptorSetLayout>, Error> {
            std::vector<VkDescriptorSetLayout> layouts {};

            for (const auto& layout : description.descriptor_layouts)
            {
                auto expected_layout = getVulkanDescriptorSetLayout(layout);
                if (!expected_layout.has_value())
                {
                    return unexpected( expected_layout.unwrap_error() );
                }
                layouts.push_back(expected_layout.unwrap());
            }

            return ok(layouts);
        }();

        if (!expected_descriptor_sets.has_value())
        {
            return unexpected( expected_descriptor_sets.unwrap_error() );
        }
        auto descriptor_sets = expected_descriptor_sets.unwrap();

        VkPipelineLayoutCreateInfo create_info {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount = static_cast<std::uint32_t>(descriptor_sets.size()),
            .pSetLayouts = descriptor_sets.data(),
            .pushConstantRangeCount = static_cast<std::uint32_t>(push_constants.size()),
            .pPushConstantRanges = push_constants.data(),
        };

        const VkResult create_result = vkCreatePipelineLayout(vk_device->handle(), &create_info, nullptr, &pipeline_layout._handle);
        if (create_result != VK_SUCCESS)
        {
            return unexpected( Error("vkCreatePipelineLayout != VK_SUCCESS") );
        }
        
        return ok(pipeline_layout);
    }

    auto PipelineLayout::destroy() noexcept -> void
    {
        log::trace("Destroying vulkan pipeline layout...");
        vkDestroyPipelineLayout(_device, _handle, nullptr);
        log::trace("Destroyed.");
    }
    
    auto getVulkanDescriptorSetLayout(const xgpu::DescriptorSetLayout& layout) noexcept -> expected<VkDescriptorSetLayout, Error>
    {
        auto* vk_descriptor_set_layout = dynamic_cast<vk::DescriptorSetLayout*>(layout.handle());
        if (!vk_descriptor_set_layout)
        {
            return unexpected( Error("Failed to get vk::DescriptorSetLayout from xgpu::DescriptorSetLayout") );
        }

        return ok(vk_descriptor_set_layout->handle());
    }
    
    auto getVulkanPushConstantRange(const xgpu::PushConstantRange& push_constant) noexcept -> VkPushConstantRange
    {
        return VkPushConstantRange {
            .stageFlags = getVulkanShaderStageFlags(push_constant.shader_stage),
            .offset = push_constant.offset,
            .size = push_constant.size,
        };
    }
} // namespace xgpu::vk
