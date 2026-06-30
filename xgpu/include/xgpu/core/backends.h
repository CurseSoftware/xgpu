#pragma once
#include "platform_detection.h"
#include "platforms.h"

namespace xgpu
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
#elif defined(XGPU_PLATFORM_WIN32)
            GraphicsApi::Vulkan
#endif
        ;

    /// @brief Constant time function to get the default graphics API
    consteval GraphicsApi
    default_graphics_api() noexcept
    {
        return DefaultGraphicsApi;
    }

    /// @brief Determine a specific GraphicsAPI is available
    template <GraphicsApi GAPI>
    consteval bool
    backend_available()
    {
        if constexpr ( GAPI == GraphicsApi::Metal ) {
#ifdef XGPU_COMPILE_METAL
            return true;
#else
            return false;
#endif // XGPU_COMPILE_METAL
        } else if ( GAPI == GraphicsApi::Vulkan ) {
#ifdef XGPU_COMPILE_VULKAN
            return true;
#else
            return false;
#endif // XGPU_COMPILE_METAL
        }

        return false;
    }

    /// @brief Determine if we are using the MoltenVK adapter layer rather than raw Vulkan
    // TODO: this may have to change depending on how MoltenVK is used with iOS and other Metal friends
    consteval bool
    is_molten_vk()
    {
        return platform_available<Platform::MacOS>() && backend_available<GraphicsApi::Vulkan>();
    }
} // namespace xgpu