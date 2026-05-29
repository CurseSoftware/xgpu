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
    };

    static_assert(
        ValidNativeTraits<type_traits<GraphicsApi::Vulkan>>, "type_traits for Vulkan API not satisfied");
} // namespace xgpu

#endif