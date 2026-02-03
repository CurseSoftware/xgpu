#ifndef RHI_VK_RENDERPASS_H
#define RHI_VK_RENDERPASS_H

#include <vulkan/vulkan_core.h>
#ifdef RHI_COMPILE_VULKAN_BACKEND

#include "rhi/vk/core.h"
#include "rhi/vk/device.h"
#include "rhi/expected.h"
#include "rhi/error.h"
#include "rhi/renderpass.h"

#include <span>

namespace rhi::vk
{
    class Renderpass
    {
        public:
            // Create a renderpass from the Open specifiers
            [[nodiscard]] static auto from_open(
                rhi::Device& device,
                std::span<OpenAttachmentDescription> attachments,
                std::span<OpenSubpassDescription> subpasses
            ) -> expected<rhi::vk::Renderpass, Error>;
        private:
            [[nodiscard]] explicit Renderpass(VkDevice device)
                : _device{ device }
            {}

        private:
            VkDevice _device { VK_NULL_HANDLE };
    };
    
    auto getVulkanAttachmentLoadOp(LoadOperation op) -> VkAttachmentLoadOp;
    auto getVulkanAttachmentStoreOp(StoreOperation op) -> VkAttachmentStoreOp;
    auto getVulkanAttachmentLayout(ImageLayout layout) -> VkImageLayout;
    auto getVulkanAttachmentType(AttachmentType type) -> VkImageLayout;
    auto getVulkanPipelineBindPoint(SubpassBindPoint bind_point) -> VkPipelineBindPoint;
} // namespace rhi::vk

#endif // RHI_COMPILE_VULKAN_BACKEND
#endif // RHI_VK_RENDERPASS_H
