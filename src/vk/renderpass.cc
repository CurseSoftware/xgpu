#include "rhi/vk/renderpass.h"
#include "platform.h"
#include "renderpass.h"
#include "vk/format.h"
#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace rhi::vk
{
    auto Renderpass::from_open(
        rhi::Device& p_device,
        std::span<OpenAttachmentDescription> p_attachments,
        std::span<OpenSubpassDescription> p_subpasses
    ) -> expected<rhi::vk::Renderpass, Error>
    {
        std::vector<VkAttachmentDescription> attachments {};
        if (p_device.backend() != Backend::Vulkan)
        {
            return unexpected( Error("Cannot create Vulkan renderpass from non-vulkan device.") );
        }

        auto* vulkan_device = dynamic_cast<vk::Device*>(p_device.handle());
        if (!vulkan_device)
        {
            return unexpected( Error("Failed to get vulkan device handle from provided rhi::Device&") );
        }
        
        for (std::size_t i = 0; i < p_attachments.size(); i++)
        {
            std::unordered_map<AttachmentType, std::vector<VkAttachmentReference>> attachment_referneces {};
            const auto& attachment = p_attachments[i];
            attachments.emplace_back( VkAttachmentDescription {
                .format = convertFormat(attachment.format),
                .loadOp = getVulkanAttachmentLoadOp(attachment.load_operation),
                .storeOp = getVulkanAttachmentStoreOp(attachment.store_operation),
                .stencilLoadOp = getVulkanAttachmentLoadOp(attachment.stencil.load_operation),
                .stencilStoreOp = getVulkanAttachmentStoreOp(attachment.stencil.store_operation),
                .initialLayout = getVulkanAttachmentLayout(attachment.initial_layout),
                .finalLayout = getVulkanAttachmentLayout(attachment.final_layout),
            });
        }

        std::vector<VkSubpassDescription> subpasses {};
        std::vector<std::unordered_map<AttachmentType, std::vector<VkAttachmentReference>>> subpass_infos;
        for (const auto& subpass : p_subpasses)
        {
            std::unordered_map<AttachmentType, std::vector<VkAttachmentReference>> attachment_references {};
            for (const auto& attachment : subpass.attachments)
            {
                attachment_references[attachment.type].emplace_back( VkAttachmentReference {
                    .attachment = attachment.index,
                    .layout = getVulkanAttachmentType(attachment.type)
                });

                subpass_infos.push_back(attachment_references);
                subpasses.emplace_back( VkSubpassDescription {
                    .pipelineBindPoint = getVulkanPipelineBindPoint(subpass.bind_point),
                    .inputAttachmentCount = static_cast<std::uint32_t>(attachment_references[AttachmentType::ShaderReadOnly].size()),
                    .pInputAttachments = attachment_references[AttachmentType::ShaderReadOnly].data(),
                    .colorAttachmentCount = static_cast<std::uint32_t>(attachment_references[AttachmentType::Color].size()),
                    .pColorAttachments = attachment_references[AttachmentType::Color].data(),

                    // This should always be 1
                    // TODO: add a check for this
                    .pDepthStencilAttachment = attachment_references[AttachmentType::DepthStencil].data()
                });
            }
        }
    }
    
    auto getVulkanAttachmentLoadOp(LoadOperation op) -> VkAttachmentLoadOp
    {
        switch (op)
        {
            case rhi::LoadOperation::Load: return VK_ATTACHMENT_LOAD_OP_LOAD;
            case rhi::LoadOperation::Clear: return VK_ATTACHMENT_LOAD_OP_CLEAR;
            case rhi::LoadOperation::DontCare: return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        }
    }
    
    auto getVulkanAttachmentStoreOp(StoreOperation op) -> VkAttachmentStoreOp
    {
        switch (op)
        {
            case rhi::StoreOperation::Store: return VK_ATTACHMENT_STORE_OP_STORE;
            case rhi::StoreOperation::DontCare: return VK_ATTACHMENT_STORE_OP_DONT_CARE;
        }
    }
    
    auto getVulkanAttachmentLayout(ImageLayout layout) -> VkImageLayout
    {
        switch (layout)
        {
            case ImageLayout::Present: return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            case ImageLayout::Undefined: return VK_IMAGE_LAYOUT_UNDEFINED;
            case ImageLayout::ColorOptimal: return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            case ImageLayout::TransferSrc: return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            case ImageLayout::TransferDst: return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        }
    }
    
    auto getVulkanAttachmentType(AttachmentType type) -> VkImageLayout
    {
        switch (type)
        {
            case AttachmentType::Color: return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            case AttachmentType::DepthStencil: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            case AttachmentType::ReadOnly: return VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
            case AttachmentType::ShaderReadOnly: return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        }
    }
    
    auto getVulkanPipelineBindPoint(SubpassBindPoint bind_point) -> VkPipelineBindPoint
    {
        switch (bind_point)
        {
            case SubpassBindPoint::Graphics: return VK_PIPELINE_BIND_POINT_GRAPHICS;
            case SubpassBindPoint::Compute: return VK_PIPELINE_BIND_POINT_COMPUTE;
        }
    }
} // namespace rhi::vk
