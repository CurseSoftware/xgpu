#ifndef RHI_VK_SWAPCHAIN_H
#define RHI_VK_SWAPCHAIN_H

#include "data/surface_info.h"
#include "rhi/expected.h"
#include "rhi/swapchain.h"
#include "rhi/vk/core.h"
#include "rhi/vk/swapchain_context.h"
#include <vulkan/vulkan_core.h>

namespace xgpu::vk
{
    class SwapChain : ISwapChain
    {
        // Factory
        public:
            auto create(const vk::SwapChainContext& ctx) noexcept -> expected<SwapChain, Error>;

        // API
        public:
            auto destroy() noexcept -> void override;

        // Private members
        private:
            VkInstance _instance       { VK_NULL_HANDLE };
            VkSurfaceKHR _surface      { VK_NULL_HANDLE };
            VkSwapchainKHR _swap_chain { VK_NULL_HANDLE };
            VkAllocationCallbacks* _allocator { nullptr };
    };

    auto getPlatformSurface(VkInstance instance, const data::SurfaceInfo& surface_info, VkAllocationCallbacks* allocator = nullptr) noexcept -> VkSurfaceKHR;
} // namespace xgpu::vk

#endif // RHI_VK_SWAPCHAIN_H
