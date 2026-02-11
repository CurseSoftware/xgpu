#ifndef RHI_VK_PIPELINE_LAYOUT_H
#define RHI_VK_PIPELINE_LAYOUT_H

#include "rhi/pipeline_layout.h"
#include "rhi/vk/core.h"
#include <vulkan/vulkan_core.h>

namespace xgpu::vk
{
    class PipelineLayout : public IPipelineLayout
    {
        public:
            [[nodiscard]] static auto from_open(xgpu::Device& device, const xgpu::PipelineLayoutDescription& description) noexcept -> expected<vk::PipelineLayout, Error>;

        // API
        public:
            [[nodiscard]] auto handle() const noexcept -> VkPipelineLayout { return _handle; }

            auto destroy() noexcept -> void override;

        private:
            [[nodiscard]] explicit PipelineLayout() noexcept = default;
            [[nodiscard]] explicit PipelineLayout(VkDevice device) noexcept 
                : _device{ device }
            {}

        private:
            VkDevice _device         { VK_NULL_HANDLE };
            VkPipelineLayout _handle { VK_NULL_HANDLE };
    };
    
    [[nodiscard]] auto getVulkanPushConstantRange(const xgpu::PushConstantRange&) noexcept -> VkPushConstantRange;
} // namespace xgpu::vk

#endif // RHI_VK_PIPELINE_LAYOUT_H
