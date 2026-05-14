#pragma once
#include "xgpu/core/core.h"

#include <string_view>

namespace xgpu
{
    /// @brief Used to create an instance
    struct InstanceDesc
    {
        std::string_view name;
    };
}