#ifndef RHI_VK_PRIV_VK_UTILS_H
#define RHI_VK_PRIV_VK_UTILS_H

#include "xgpu/types.h"
#include "xgpu/vk/core.h"
#include <vulkan/vulkan_core.h>

namespace xgpu::vk
{
    auto convertSharingMode(SharingMode) -> VkSharingMode;

    auto convertImageTiling(ImageTiling) -> VkImageTiling;

    auto getVulkanImageUsage(ImageUsage) -> VkImageUsageFlags;

    auto convertImageUsage(ImageUsage) -> VkImageUsageFlags;

    auto convertImageType(ImageType) -> VkImageType;
    
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

    auto getVulkanAttachmentLoadOp(LoadOperation) -> VkAttachmentLoadOp;

    auto getVulkanAttachmentStoreOp(StoreOperation) -> VkAttachmentStoreOp;

    auto getVulkanAttachmentLayout(ImageLayout) -> VkImageLayout;

    auto getVulkanAttachmentType(AttachmentType) -> VkImageLayout;

    auto getVulkanPipelineBindPoint(SubpassBindPoint) -> VkPipelineBindPoint;

    auto getVulkanPipelineBindPoint(PipelineBindPoint) -> VkPipelineBindPoint;

    auto convertImageLayout(ImageLayout) -> VkImageLayout;

    auto convertImageAspect(ImageAspectFlags) -> VkImageAspectFlags;

    auto convertBufferUsage(BufferUsageFlags) -> VkBufferUsageFlags;

    auto converBufferUsageFlagBits(BufferUsageFlagBits) -> VkBufferUsageFlagBits;

    auto convertPipelineStage(PipelineStage) -> VkPipelineStageFlags;
} // namespace xgpu::vk

#endif // RHI_VK_PRIV_VK_UTILS_H
