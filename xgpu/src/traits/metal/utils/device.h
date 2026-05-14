#pragma once
#include "xgpu/data/data.h"
#include "xgpu/traits/metal/metal.h"
#include <vector>

namespace xgpu::mtl
{
    std::vector<data::PhysicalDevice> get_available_devices() noexcept;
} // namespace xgpu::mtl
