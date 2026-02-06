#ifndef RHI_VK_FRAMEBUFFER_H
#define RHI_VK_FRAMEBUFFER_H

#include "rhi/vk/core.h"
#include "rhi/framebuffer.h"
#include "rhi/error.h"
#include "rhi/expected.h"
#include "rhi/device.h"

namespace rhi::vk
{
    class Framebuffer : public IFramebuffer
    {
        // Factory
        public:
            // Create a vulkan framebuffer object from the rhi open interface
            static auto from_open(rhi::Device& device, const FramebufferDescription& description) noexcept -> expected<vk::Framebuffer, Error>;

        // API
        public:
            auto destroy() noexcept -> void override;

        // Private special members
        private:
            [[nodiscard]] explicit Framebuffer(VkDevice device) noexcept
                : _device{ device }
            {}
        
        // Private fields
        private:
            VkDevice _device      { VK_NULL_HANDLE };
            VkFramebuffer _handle { VK_NULL_HANDLE };
    };
} // namespace rhi::vk

#endif // RHI_VK_FRAMEBUFFER_H
