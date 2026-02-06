#include "rhi/vk/framebuffer.h"
#include "core/log.h"
#include "rhi/vk/device.h"
#include "expected.h"
#include "vk/renderpass.h"
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

        Framebuffer framebuffer { vk_device->handle() };

        VkFramebufferCreateInfo create_info {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = vk_renderpass->handle(),
            .width = description.width,
            .height = description.height,
        };

        return ok(framebuffer);
    }

    auto Framebuffer::destroy() noexcept -> void
    {
        log::trace("Destroying vulkan framebuffer...");
        vkDestroyFramebuffer(_device, _handle, nullptr);
        log::trace("Destroyed.");
    }
} // namespace rhi::vk
