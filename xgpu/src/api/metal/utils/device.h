#pragma once
#include "xgpu/core/core.h"
#include "xgpu/types/types.h"

#ifdef XGPU_COMPILE_METAL
#include "../metal_headers.h"
#include "xgpu/data/data.h"

#include <vector>

namespace xgpu::metal
{
    std::vector<data::PhysicalDevice>                      get_available_physical_devices() noexcept;

    type_traits<GraphicsApi::Metal>::device_t              default_device() noexcept;

    std::vector<type_traits<GraphicsApi::Metal>::device_t> all_devices() noexcept;
} // namespace xgpu::metal
#endif // XGPU_COMPILE_METAL
