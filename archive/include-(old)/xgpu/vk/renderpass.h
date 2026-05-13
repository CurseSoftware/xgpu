#ifndef RHI_VK_RENDERPASS_H
#define RHI_VK_RENDERPASS_H

#include <vulkan/vulkan_core.h>
#ifdef RHI_COMPILE_VULKAN_BACKEND

#include "xgpu/vk/core.h"
#include "xgpu/vk/device.h"
#include "xgpu/expected.h"
#include "xgpu/error.h"
#include "xgpu/renderpass.h"

#include <span>

namespace xgpu::vk
{
    class Renderpass : public IRenderpass
    {
        public:
            // Create a renderpass from the Open specifiers
            [[nodiscard]] static auto from_open(
                xgpu::Device& device,
                std::span<OpenAttachmentDescription> attachments,
                std::span<OpenSubpassDescription> subpasses
            ) -> expected<xgpu::vk::Renderpass, Error>;

        // API
        public:
            auto destroy() noexcept -> void override;

            [[nodiscard]] auto handle() const noexcept -> VkRenderPass { return _renderpass; }
        
        private:
            [[nodiscard]] explicit Renderpass(VkDevice device) 
                : _device{ device } 
            {}

        private:
            VkDevice _device         { VK_NULL_HANDLE };
            VkRenderPass _renderpass { VK_NULL_HANDLE };
    };
    
} // namespace xgpu::vk

#endif // RHI_COMPILE_VULKAN_BACKEND
#endif // RHI_VK_RENDERPASS_H
