#ifndef RHI_VK_DESCRIPTOR_SET_H
#define RHI_VK_DESCRIPTOR_SET_H

#include "rhi/expected.h"
#include "rhi/device.h"
#include "rhi/descriptor_set.h"

namespace xgpu::vk
{
    class DescriptorSetLayout : public IDescriptorSetLayout
    {
        // Factory
        public:
            // Create a vulkan descriptor set layout from the open rhi API
            [[nodiscard]] static auto from_open(xgpu::Device& device, const xgpu::DescriptorSetLayoutDescription& description) noexcept -> expected<vk::DescriptorSetLayout, Error>;

        // API
        public:
            // Destroy the descriptor set layout
            auto destroy() noexcept -> void override;

            // Get a handle to the descriptor set layout held
            [[nodiscard]] auto handle() const noexcept -> VkDescriptorSetLayout { return _handle; }

        // Special members
        private:
            [[nodiscard]] explicit DescriptorSetLayout(VkDevice device) noexcept
                : _device{ device }
            {}
        
        // Private fields
        private:
            VkDevice _device              { VK_NULL_HANDLE };
            VkDescriptorSetLayout _handle { VK_NULL_HANDLE };
    };

    [[nodiscard]] auto getVulkanDescriptorSetLayout(const xgpu::DescriptorSetLayout&) noexcept -> expected<VkDescriptorSetLayout, Error>;

    [[nodiscard]] auto getVulkanDescriptorSetLayoutBinding(const xgpu::DescriptorSetLayoutBinding&) noexcept -> VkDescriptorSetLayoutBinding;
} // namespace xgpu::vk

#endif // RHI_VK_DESCRIPTOR_SET_H
