#pragma once
#include "base.h"
#include "xgpu/native/native.h"

namespace xgpu
{
    template <>
    struct type_traits<GraphicsApi::Metal>
    {
        using instance_t      = void;
        using device_t        = OBJC_TYPE(id<MTLDevice>);
        using command_queue_t = OBJC_TYPE(id<MTLCommandQueue>);
    };

    static_assert(
        ValidTypeTraits<type_traits<GraphicsApi::Metal>>, "type_traits for metal API not satisfied");
} // namespace xgpu::types