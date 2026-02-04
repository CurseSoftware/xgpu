#ifndef RHI_VK_PIPELINE_H
#define RHI_VK_PIPELINE_H

#include "rhi/expected.h"
#include "rhi/pipeline.h"
#include "rhi/device.h"
#include "rhi/vk/core.h"
#include "rhi/shader.h"

#include <unordered_map>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace rhi::vk
{
    class Pipeline : public IPipeline
    {
        public:
            [[nodiscard]] static auto from_open(rhi::Device& p_device, const GraphicsPipelineDescription& p_description) noexcept -> expected<vk::Pipeline, Error>;

            auto destroy() noexcept -> void override;

        private:
            VkPipeline _pipeline { VK_NULL_HANDLE };
    };

    // Get a list of vulkan shader stage create infos from the rhi::ShaderStage's
    auto getVulkanShaderStageInfos(const std::unordered_map<ShaderStageFlags, std::reference_wrapper<rhi::ShaderModule>>& shader_modules) -> expected<std::vector<VkPipelineShaderStageCreateInfo>, Error>;

    // Get a vulkan shader stage info from the rhi interface and native module
    auto getVulkanShaderStageInfo(ShaderStageFlags stage, VkShaderModule module) -> VkPipelineShaderStageCreateInfo;

    // Get the vulkan shader stage flags from the rhi shader stage flags
    auto getVulkanShaderStageFlags(ShaderStageFlags stage) -> VkShaderStageFlagBits;

    // Get the vulkan input assembly state info from the rhi interface description
    auto getVulkanInputAssemblyState(const InputAssemblyStateDescription& desc) -> VkPipelineInputAssemblyStateCreateInfo;
    
    // Get the vulkan rasterization state info from the rhi interface description
    auto getVulkanRasterizationState(const RasterizationStateDescription& desc) -> VkPipelineRasterizationStateCreateInfo;

    // Get the vulkan multisample state from the rhi interface description
    auto getVulkanMultisampleState(const MultisampleStateDescription& desc) -> VkPipelineMultisampleStateCreateInfo;

    // Get the vulkan stencil op state from rhi interface description
    auto getVulkanStencilOp(const StencilOpState& desc) -> VkStencilOpState;

    // Get the vulkan depth stencil state from the rhi interface description
    auto getVulkanDepthStencil(const DepthStencilStateDescription& desc) -> VkPipelineDepthStencilStateCreateInfo;

    // Get list of vulkan color blend states from rhi interface
    auto getVulkanColorBlendAttachment(std::span<ColorBlendAttachmentStateDescription> attachments) -> std::vector<VkPipelineColorBlendAttachmentState>;

    // Get the vulkan color blend state from rhi description
    auto getVulkanColorBlendState(const ColorBlendStateDescription& desc, std::span<VkPipelineColorBlendAttachmentState> attachments) -> VkPipelineColorBlendStateCreateInfo;

    // Get a vulkan dynamic state from the rhi description
    auto getVulkanDynamicState(const DynamicStateDescription& desc) -> VkDynamicState;

    // Get a list of vulkan dynamic states from the rhi descriptions
    auto getVulkanDynamicStates(std::span<DynamicStateDescription> states) -> std::vector<VkDynamicState>;

    // Get the vulkan vertex input info from the rhi description
    auto getVulkanVertexInputState(const VertexInputDescription& desc) -> VkPipelineVertexInputStateCreateInfo;

    // Get the vulkan vertex input info from the rhi description
    auto getVulkanVertexInputState(const TesselationStateDescription& desc) -> VkPipelineVertexInputStateCreateInfo;

    // Get a list of vulkan viewports from rhi descriptions
    auto getVulkanViewports(std::span<ViewportDescription> viewports) -> std::vector<VkViewport>;

    // Get vulkan viewport from rhi description
    auto getVulkanViewport(const ViewportDescription& viewport) -> VkViewport;
    
    // Get vulkan scissor from rhi description
    auto getVulkanScissor(const ScissorDescription& scissor) -> VkRect2D;

    // Get a list of vulkan scissors from rhi descriptions
    auto getVulkanScissors(std::span<ScissorDescription> scissors) -> std::vector<VkRect2D>;

    
} // namespace rhi::vk

#endif // RHI_VK_PIPELINE_H
