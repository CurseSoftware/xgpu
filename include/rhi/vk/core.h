#ifndef RHI_VK_CORE_H
#define RHI_VK_CORE_H

#ifdef RHI_COMPILE_VULKAN_BACKEND

#include "rhi/vk/instance.h"
#include "rhi/platform.h"

#ifdef RHI_PLATFORM_LINUX
#define VK_USE_PLATFORM_XLIB_KHR

#include <X11/Xlib.h>
#include <vulkan/vulkan_xlib.h>

#endif // Platform detection

#include <vulkan/vulkan.h>

namespace rhi::vk
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
    
    template <typename T, typename U>
    auto getNativeHandle(T& t) -> U;

    template <>
    inline auto getNativeHandle(rhi::vk::Instance& inst) -> VkInstance
    {
        return inst.native_handle();
    }

} // namespace rhi::vk

#endif // RHI_COMPILE_VULKAN_BACKEND
#endif // RHI_VK_CORE_H
