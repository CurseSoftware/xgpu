#pragma once
#include <rtl/ext/view.h>

namespace xgpu
{
    /// @brief Used for a read-only view into an object or its data
    template <typename T>
    using view = rtl::ext::view<T>;
}