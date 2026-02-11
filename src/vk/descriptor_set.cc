#include "rhi/vk/descriptor_set.h"
#include "core/log.h"
#include "rhi/vk/device.h"
#include "vk/pipeline.h"
#include "vk/vk_utils.h"
#include <vulkan/vulkan_core.h>

namespace xgpu::vk
{
    auto DescriptorSetLayout::from_open(xgpu::Device& device, const xgpu::DescriptorSetLayoutDescription& description) noexcept -> expected<vk::DescriptorSetLayout, Error>
    {
        auto* vk_device = dynamic_cast<vk::Device*>(device.handle());
        if (!vk_device)
        {
            return unexpected( Error("Failed to get vulkan device from xgpu::Device") );
        }

        vk::DescriptorSetLayout descriptor_set { vk_device->handle() };

        auto bindings = [&] {
            std::vector<VkDescriptorSetLayoutBinding> bindings {};

            for (const auto& binding : description.bindings)
            {
                bindings.push_back(getVulkanDescriptorSetLayoutBinding(binding));
            }

            return bindings;
        }();

        VkDescriptorSetLayoutCreateInfo create_info {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .bindingCount = static_cast<std::uint32_t>(bindings.size()),
            .pBindings = bindings.data(),
        };

        const VkResult create_result = vkCreateDescriptorSetLayout(vk_device->handle(), &create_info, nullptr, &descriptor_set._handle);
        if (create_result != VK_SUCCESS)
        {
            return unexpected( Error("vkCreateDescriptorSetLayout != VK_SUCCESS") );
        }

        return ok(descriptor_set);
    }

    auto DescriptorSetLayout::destroy() noexcept -> void
    {
        log::trace("Destroying vulkan descriptor set...");
        vkDestroyDescriptorSetLayout(_device, _handle, nullptr);
        log::trace("Destroyed.");
    }
    
    auto getVulkanDescriptorSetLayoutBinding(const xgpu::DescriptorSetLayoutBinding& desc) noexcept -> VkDescriptorSetLayoutBinding
    {
        return VkDescriptorSetLayoutBinding {
            .binding = desc.binding,
            .descriptorType = convertDescriptorType(desc.descriptor_type),
            .descriptorCount = desc.descriptor_count,
            .stageFlags = getVulkanShaderStageFlags(desc.shader_stage),
        };
    }
} // namespace xgpu::vk
