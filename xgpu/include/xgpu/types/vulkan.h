#pragma once
#include "metal.h"
#include "xgpu/core/core.h"
#include "xgpu/native/native.h"

#ifdef XGPU_COMPILE_VULKAN

namespace xgpu
{
    template <>
    struct type_traits<GraphicsApi::Vulkan>
    {
        using none_t          = types::Empty;
        using instance_t      = VkInstance;
        using device_t        = VkDevice;
        using command_queue_t = VkQueue;
        using debug_manager_t = VkDebugUtilsMessengerEXT;
    };

    static_assert(
        ValidNativeTraits<type_traits<GraphicsApi::Vulkan>>, "type_traits for Vulkan API not satisfied");
} // namespace xgpu

#endif