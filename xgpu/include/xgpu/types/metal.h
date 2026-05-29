#pragma once
#include "base.h"
#include "xgpu/native/native.h"

namespace xgpu
{
    template <>
    struct type_traits<GraphicsApi::Metal>
    {
        using none_t          = types::Empty;
        using instance_t      = none_t;
        using device_t        = OBJC_TYPE(id<MTLDevice>);
        using command_queue_t = OBJC_TYPE(id<MTLCommandQueue>);
        using debug_manager_t = none_t;
    };

    static_assert(
        ValidNativeTraits<type_traits<GraphicsApi::Metal>>, "type_traits for metal API not satisfied");
} // namespace xgpu::types