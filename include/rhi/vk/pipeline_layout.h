#ifndef RHI_VK_PIPELINE_LAYOUT_H
#define RHI_VK_PIPELINE_LAYOUT_H

#include "rhi/pipeline_layout.h"
#include "rhi/vk/core.h"
#include <vulkan/vulkan_core.h>

namespace rhi::vk
{
    class PipelineLayout : public IPipelineLayout
    {
        public:
            [[nodiscard]] static auto from_open(rhi::Device& device, const rhi::PipelineLayoutDescription& description) noexcept -> expected<vk::PipelineLayout, Error>;

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
    
    [[nodiscard]] auto getVulkanPushConstantRange(const rhi::PushConstantRange&) noexcept -> VkPushConstantRange;
} // namespace rhi::vk

#endif // RHI_VK_PIPELINE_LAYOUT_H
