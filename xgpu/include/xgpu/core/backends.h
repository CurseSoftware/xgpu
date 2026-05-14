#pragma once
#include "platform_detection.h"

namespace xgpu::core
{
    /// @brief Enumeration of supported graphics APIs
    enum class GraphicsApi
    {
        Metal,
        Vulkan,
        Dx12
    };

    constexpr static GraphicsApi DefaultGraphicsApi =
#if defined(XGPU_PLATFORM_MACOS)
        GraphicsApi::Metal
#endif
        ;

    /// @brief Constant time function to get the default graphics API
    consteval GraphicsApi
    default_graphics_api() noexcept
    {
        return DefaultGraphicsApi;
    }
} // namespace xgpu::core