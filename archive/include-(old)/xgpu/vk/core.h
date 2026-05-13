#ifndef RHI_VK_CORE_H
#define RHI_VK_CORE_H

#ifdef RHI_COMPILE_VULKAN_BACKEND

// #include "xgpu/vk/instance.h"
#include "xgpu/platform.h"

#ifdef RHI_PLATFORM_LINUX
#define VK_USE_PLATFORM_XLIB_KHR

#include <X11/Xlib.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xlib.h>

#endif // Platform detection


namespace xgpu::vk
{
    constexpr const char* DEBUG_EXTENSION_NAME { VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
    constexpr const char* DEBUG_LAYER_NAME { "VK_LAYER_KHRONOS_validation" };
    
    constexpr const char* PLATFORM_SURFACE_NAME { 
#ifdef RHI_PLATFORM_WINDOWS
        "VK_KHR_xlib_surface"
#elif defined(RHI_PLATFORM_LINUX)
        "VK_KHR_xlib_surface"
#endif
    };
} // namespace xgpu::vk

#endif // RHI_COMPILE_VULKAN_BACKEND
#endif // RHI_VK_CORE_H
