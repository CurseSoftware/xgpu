#ifndef RHI_VK_CORE_H
#define RHI_VK_CORE_H

#include "instance.h"
#include "rhi/platform.h"

#include <vulkan/vulkan_core.h>

namespace rhi::vk
{
    constexpr const char* DEBUG_EXTENSION_NAME { VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
    constexpr const char* DEBUG_LAYER_NAME { "VK_LAYER_KHRONOS_validation" };
    
    constexpr const char* PLATFORM_SURFACE_NAME { 
#ifdef RHI_PLATFORM_WINDOWS
#elif defined(RHI_PLATFORM_LINUX)
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

#endif // RHI_VK_CORE_H
