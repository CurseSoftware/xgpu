#pragma once

#include "xgpu/core/core.h"

namespace xgpu
{
    template <typename T>
    concept ValidTypeTraits = requires {
        typename T::instance_t;
        typename T::device_t;
        typename T::command_queue_t;
    };

    template <GraphicsApi GAPI>
    struct type_traits;

    template <GraphicsApi GAPI = DefaultGraphicsApi>
    using native_types = type_traits<GAPI>;
} // namespace xgpu