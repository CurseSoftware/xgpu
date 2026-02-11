#ifndef RHI_VK_FRAMEBUFFER_H
#define RHI_VK_FRAMEBUFFER_H

#include "xgpu/vk/core.h"
#include "xgpu/framebuffer.h"
#include "xgpu/error.h"
#include "xgpu/expected.h"
#include "xgpu/device.h"
#include <vulkan/vulkan_core.h>

namespace xgpu::vk
{
    class Framebuffer : public IFramebuffer
    {
        // Factory
        public:
            // Create a vulkan framebuffer object from the xgpu open interface
            static auto from_open(xgpu::Device& device, const FramebufferDescription& description) noexcept -> expected<vk::Framebuffer, Error>;

        // API
        public:
            auto destroy() noexcept -> void override;

            [[nodiscard]] auto handle() noexcept -> VkFramebuffer { return _handle; }

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
} // namespace xgpu::vk

#endif // RHI_VK_FRAMEBUFFER_H
