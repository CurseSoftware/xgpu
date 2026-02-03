#ifndef RHI_VK_SWAPCHAIN_CONTEXT_H
#define RHI_VK_SWAPCHAIN_CONTEXT_H

#include <vulkan/vulkan_core.h>
#ifdef RHI_COMPILE_VULKAN_BACKEND
#include "rhi/data/surface_info.h"
#include "rhi/vk/core.h"


namespace rhi::vk
{
    struct SwapChainContext
    {
        VkInstance instance              { VK_NULL_HANDLE };
        VkAllocationCallbacks* allocator { nullptr };
        
        data::SurfaceInfo surface_info;
    };
} // namespace rhi::vk

#endif // RHI_COMPILE_VULKAN_BACKEND
#endif // RHI_VK_SWAPCHAIN_CONTEXT_H
