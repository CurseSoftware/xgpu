#ifndef RHI_VK_PRIV_VK_UTILS_H
#define RHI_VK_PRIV_VK_UTILS_H

#include "rhi/types.h"
#include "rhi/vk/core.h"
#include <vulkan/vulkan_core.h>

namespace rhi::vk
{
    auto convertTopology(Topology) -> VkPrimitiveTopology;
    
    auto convertPolygonMode(PolygonMode) -> VkPolygonMode;

    auto convertCullMode(CullMode) -> VkCullModeFlagBits;

    auto convertFrontFace(FrontFace) -> VkFrontFace;

    auto convertBlendOperator(BlendOperator) -> VkBlendOp;

    auto convertBlendFactor(BlendFactor) -> VkBlendFactor;

    auto convertColorComponentFlags(ColorComponentFlags) -> VkColorComponentFlags;

    auto convertDynamicState(PipelineDynamicState) -> VkDynamicState;

    auto convertLogicOperator(LogicOperator) -> VkLogicOp;
    
    auto convertStencilOperator(StencilOperator) -> VkStencilOp;
    
    auto convertCompareOperator(CompareOperator) -> VkCompareOp;

    auto convertDescriptorType(DescriptorType) -> VkDescriptorType;

    auto convertSampleCount(SampleCount) -> VkSampleCountFlagBits;
} // namespace rhi::vk

#endif // RHI_VK_PRIV_VK_UTILS_H
