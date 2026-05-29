#pragma once

#include "xgpu/core/core.h"

namespace xgpu
{
    namespace types
    {
        /// @brief Used as an empty struct
        /// @note This is mainly used so that backends that do not include a certain type
        /// (Metal does not have instance_t) can still define this type without compilation error
        struct Empty
        {
        };
    } // namespace types

    template <typename T>
    concept ValidNativeTraits = requires {
        typename T::none_t;
        typename T::instance_t;
        typename T::device_t;
        typename T::command_queue_t;
    };

    template <GraphicsApi GAPI>
    struct type_traits;

    template <GraphicsApi GAPI = DefaultGraphicsApi>
    using native_types = type_traits<GAPI>;
} // namespace xgpu