#include "rhi/vk/framebuffer.h"
#include "core/log.h"
#include "rhi/vk/device.h"
#include "expected.h"
#include "vk/image_view.h"
#include "vk/renderpass.h"
#include <array>
#include <vulkan/vulkan_core.h>

namespace rhi::vk
{
    auto Framebuffer::from_open(rhi::Device &device, const FramebufferDescription &description) noexcept -> expected<vk::Framebuffer, Error>
    {
        auto* vk_device = dynamic_cast<vk::Device*>(device.handle());
        if (!vk_device)
        {
            return unexpected( Error("Failed to get vk::Device from rhi::Device") );
        }

        auto* vk_renderpass = dynamic_cast<vk::Renderpass*>(description.renderpass.handle());
        if (!vk_renderpass)
        {
            return unexpected( Error("Failed to get vk::Renderpass from rhi::Renderpass") );
        }

        std::uint32_t layer_count { 0 };
        auto expected_attachments = [&]() -> expected<std::vector<VkImageView>, Error> {
            std::vector<VkImageView> attachments {};

            for (const auto& attachment : description.attachments)
            {
                auto* vk_image_view = dynamic_cast<vk::ImageView*>(attachment.get().handle());
                if (!vk_image_view)
                {
                    log::error("HERE");
                    return unexpected( Error("Failed to get vk::ImageView from rhi::ImageView") );
                }

                if (layer_count != 0 && vk_image_view->layer_count() != layer_count)
                {
                    log::error("HERE2");
                    return unexpected( Error("All vulkan image views attached to a framebuffer must have same number of layers") );
                }
                layer_count = vk_image_view->layer_count();
                attachments.push_back(vk_image_view->handle());
            }

            return ok(attachments);
        }();
        
        if (!expected_attachments.has_value())
        {
            log::error("Got here");
            return unexpected( expected_attachments.unwrap_error() );
        }
        auto attachments = expected_attachments.unwrap();

        Framebuffer framebuffer { vk_device->handle() };

        VkFramebufferCreateInfo create_info {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = vk_renderpass->handle(),
            .attachmentCount = static_cast<std::uint32_t>(attachments.size()),
            .pAttachments = attachments.data(),
            .width = description.width,
            .height = description.height,
            .layers = layer_count,
        };

        const VkResult result = vkCreateFramebuffer(vk_device->handle(), &create_info, nullptr, &framebuffer._handle);
        if (result != VK_SUCCESS)
        {
            log::error("HERE3");
            return unexpected( Error("vkCreateFramebuffer != VK_SUCCESS") );
        }

        return ok(framebuffer);
    }

    auto Framebuffer::destroy() noexcept -> void
    {
        log::trace("Destroying vulkan framebuffer...");
        vkDestroyFramebuffer(_device, _handle, nullptr);
        log::trace("Destroyed.");
    }
} // namespace rhi::vk
