#pragma once
#include <rtl/expected.h>

namespace xgpu
{
    template <typename T, typename E>
    using expected = rtl::expected<T, E>;

    template <typename E>
    using unexpected = rtl::unexpected<E>;
}