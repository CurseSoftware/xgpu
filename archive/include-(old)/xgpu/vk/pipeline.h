#ifndef RHI_VK_PIPELINE_H
#define RHI_VK_PIPELINE_H

#include "xgpu/expected.h"
#include "xgpu/pipeline.h"
#include "xgpu/device.h"
#include "xgpu/vk/core.h"
#include "xgpu/shader.h"

#include <unordered_map>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace xgpu::vk
{
    class Pipeline : public IPipeline
    {
        public:
            [[nodiscard]] static auto from_open(xgpu::Device& p_device, const GraphicsPipelineDescription& p_description) noexcept -> expected<vk::Pipeline, Error>;

            auto destroy() noexcept -> void override;

            [[nodiscard]] auto handle() const noexcept -> VkPipeline { return _handle; }

        private:
            [[nodiscard]] explicit Pipeline(VkDevice device) noexcept
                : _device{ device }
            {}

        private:
            VkDevice _device   { VK_NULL_HANDLE };
            VkPipeline _handle { VK_NULL_HANDLE };
    };

    auto getVulkanPipelineLayout(const xgpu::PipelineLayout&) noexcept -> expected<VkPipelineLayout, Error>;

    // Get a list of vulkan shader stage create infos from the xgpu::ShaderStage's
    auto getVulkanShaderStageInfos(const std::unordered_map<ShaderStageFlags, std::reference_wrapper<xgpu::ShaderModule>>& shader_modules) -> expected<std::vector<VkPipelineShaderStageCreateInfo>, Error>;

    // Get a vulkan shader stage info from the xgpu interface and native module
    auto getVulkanShaderStageInfo(ShaderStageFlags stage, VkShaderModule module) -> VkPipelineShaderStageCreateInfo;

    // Get the vulkan shader stage flags from the xgpu shader stage flags
    auto getVulkanShaderStageFlags(ShaderStageFlags stage) -> VkShaderStageFlagBits;

    // Get the vulkan input assembly state info from the xgpu interface description
    auto getVulkanInputAssemblyState(const InputAssemblyStateDescription& desc) -> VkPipelineInputAssemblyStateCreateInfo;
    
    // Get the vulkan rasterization state info from the xgpu interface description
    auto getVulkanRasterizationState(const RasterizationStateDescription& desc) -> VkPipelineRasterizationStateCreateInfo;

    // Get the vulkan multisample state from the xgpu interface description
    auto getVulkanMultisampleState(const MultisampleStateDescription& desc) -> VkPipelineMultisampleStateCreateInfo;

    // Get the vulkan stencil op state from xgpu interface description
    auto getVulkanStencilOp(const StencilOpState& desc) -> VkStencilOpState;

    // Get the vulkan depth stencil state from the xgpu interface description
    auto getVulkanDepthStencil(const DepthStencilStateDescription& desc) -> VkPipelineDepthStencilStateCreateInfo;

    // Get list of vulkan color blend states from xgpu interface
    auto getVulkanColorBlendAttachments(std::span<const ColorBlendAttachmentStateDescription> attachments) -> std::vector<VkPipelineColorBlendAttachmentState>;

    // Get the vulkan color blend state from xgpu description
    auto getVulkanColorBlendState(const ColorBlendStateDescription& desc, std::span<VkPipelineColorBlendAttachmentState> attachments) -> VkPipelineColorBlendStateCreateInfo;

    // Get a vulkan dynamic state from the xgpu description
    auto getVulkanDynamicState(std::span<VkDynamicState>) -> VkPipelineDynamicStateCreateInfo;

    // Get a list of vulkan dynamic states from the xgpu descriptions
    auto getVulkanDynamicStates(std::span<const PipelineDynamicState> states) -> std::vector<VkDynamicState>;

    // Get the vulkan vertex input info from the xgpu description
    auto getVulkanVertexInputState(
        const VertexInputDescription& desc,
        std::span<VkVertexInputAttributeDescription> attribute_descriptions,
        std::span<VkVertexInputBindingDescription> binding_descriptions
    ) -> VkPipelineVertexInputStateCreateInfo;

    // Get the vulkan vertex attribute descriptions from the xgpu interface
    auto getVulkanVertexInputAttributeDescriptions(std::span<VertexInputDescription::AttributeDescription> attributes) -> std::vector<VkVertexInputAttributeDescription>;
    
    // Get the vulkan vertex binding descriptions from the xgpu interface
    auto getVulkanVertexInputBindingDescriptions(std::span<VertexInputDescription::Binding> bindings) -> std::vector<VkVertexInputBindingDescription>;

    // Get the vulkan vertex input info from the xgpu description
    auto getVulkanTesselationState(const TesselationStateDescription& desc) -> VkPipelineTessellationStateCreateInfo;;

    // Get a list of vulkan viewports from xgpu descriptions
    auto getVulkanViewports(std::span<const ViewportDescription> viewports) -> std::vector<VkViewport>;

    // Get vulkan viewport from xgpu description
    auto getVulkanViewport(const ViewportDescription& viewport) -> VkViewport;
    
    // Get vulkan scissor from xgpu description
    auto getVulkanScissor(const ScissorDescription& scissor) -> VkRect2D;

    // Get a list of vulkan scissors from xgpu descriptions
    auto getVulkanScissors(std::span<const ScissorDescription> scissors) -> std::vector<VkRect2D>;

    auto getVulkanViewportState(
        std::span<VkViewport> viewports,
        std::span<VkRect2D> scissors
    ) -> VkPipelineViewportStateCreateInfo;
    
} // namespace xgpu::vk

#endif // RHI_VK_PIPELINE_H
