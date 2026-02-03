#include "rhi/platform.h"
#include "rhi/vk/core.h"
#include <vulkan/vulkan_core.h>
#include "rhi/vk/swapchain.h"

namespace rhi::vk
{
    auto getPlatformSurface(VkInstance instance, const data::SurfaceInfo& surface_info, VkAllocationCallbacks* allocator) noexcept -> VkSurfaceKHR
    {
        VkSurfaceKHR surface { VK_NULL_HANDLE };

#if defined(RHI_PLATFORM_LINUX)
        VkXlibSurfaceCreateInfoKHR create_info {
            .sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0,
            .dpy = surface_info.display,
            .window = surface_info.window
        };

        const VkResult create_result = vkCreateXlibSurfaceKHR(instance, &create_info, allocator, &surface);
#elif defined(RHI_PLATFORM_WINDOWS)
#elif defined(RHI_PLATFORM_APPLE)
#endif // Platform Detection

        return surface;
    }

    auto SwapChain::create(const rhi::vk::SwapChainContext& ctx) noexcept -> expected<SwapChain, Error>
    {
        SwapChain swapchain;

        swapchain._instance = ctx.instance;
        swapchain._allocator = ctx.allocator;
        swapchain._surface = getPlatformSurface(ctx.instance, ctx.surface_info);
        if (swapchain._surface == VK_NULL_HANDLE)
        {
            return unexpected( Error("Failed to create vulkan surface") );
        }

        return ok(swapchain);
    }

    auto SwapChain::destroy() noexcept -> void
    {
        vkDestroySurfaceKHR(_instance, _surface, _allocator);
    }
} // namespace rhi::vk
