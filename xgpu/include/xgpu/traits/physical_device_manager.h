#pragma once
#include "xgpu/core/core.h"
#include "xgpu/data/data.h"

#include <concepts>
#include <span>
#include <string>

namespace xgpu::traits
{
    template <typename T>
    concept IPhysicalDevice = requires(T t) {
        { t.enumerate_devices() } -> std::same_as<std::span<data::PhysicalDevice>>;

        { t.default_physical_device() } -> std::same_as<data::PhysicalDevice>;
    };

    /// @brief Named class that should never be instantiated
    template <core::GraphicsApi GAPI>
    class PhysicalDeviceManager;
} // namespace xgpu::traits