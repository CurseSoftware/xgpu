#pragma once
#include "xgpu/data/data.h"
#include "xgpu/native/native.h"
#include "xgpu/traits/metal/metal.h"
#include <vector>

namespace xgpu::mtl
{
    std::vector<data::PhysicalDevice>                      get_available_physical_devices() noexcept;
    std::vector<type_traits<GraphicsApi::Metal>::device_t> all_devices() noexcept;
    type_traits<GraphicsApi::Metal>::device_t              default_device() noexcept;
} // namespace xgpu::mtl
