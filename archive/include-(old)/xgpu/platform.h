#ifndef RHI_PLATFORM_H
#define RHI_PLATFORM_H

namespace xgpu
{
    enum class Backend
    {
        Vulkan,
        Dx12,
        Metal
    };

    enum class Platform
    {
        Linux,
        Windows,
        MacOS,
        Other
    };

    // Return whether the Vulkan backend is available
    consteval auto vulkanAvailable() -> bool
    {
#ifdef RHI_COMPILE_VULKAN_BACKEND
                return true;
#else
                return false;
#endif // RHI_COMPILE_VULKAN_BACKEND
    }

    // Return whether the DirectX12 backend is available
    consteval auto dx12Available() -> bool
    {
#ifdef RHI_COMPILE_DX12_BACKEND 
                return true;
#else
                return false;
#endif // RHI_COMPILE_DX12_BACKEND
    }

    // Return whether the metal backend API is available
    consteval auto metalAvailable() -> bool
    {
#ifdef RHI_COMPILE_METAL_BACKEND 
                return true;
#else
                return false;
#endif // RHI_COMPILE_METAL_BACKEND
    }

    // Find whether a backend is available for the platform
    constexpr auto backendIsAvailable(Backend p_platform) noexcept -> bool
    {
        switch (p_platform)
        {
            case Backend::Vulkan:
                return vulkanAvailable();
            case Backend::Dx12:
                return dx12Available();
            case Backend::Metal:
                return metalAvailable();
        }
    }

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define RHI_PLATFORM_WINDOWS
    constexpr Platform SystemPlatform { Platform::Windows };
#elif defined(__linux) || defined(__gnu_linux__)
#define RHI_PLATFORM_LINUX
    constexpr Platform SystemPlatform { Platform::Linux };
#elif defined(__APPLE__)
#define RHI_PLATFORM_APPLE
    constexpr Platform SystemPlatform { Platform::MacOS };
#endif

    // Get the default backend for the platform
    consteval auto getDefaultBackend() noexcept -> Backend
    {
        switch (SystemPlatform)
        {
            case Platform::Windows:
                return Backend::Dx12;
            case Platform::Linux:
                return Backend::Vulkan;
            case Platform::MacOS:
                return Backend::Metal;
        }
    }
};

#endif // RHI_PLATFORM_H
