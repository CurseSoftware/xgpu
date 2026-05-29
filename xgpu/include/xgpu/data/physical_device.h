#pragma once
#include <cstdint>
#include <string>

namespace xgpu::data
{
    /// @brief The types of available physical devices
    enum class PhysicalDeviceType
    {
        DiscreteGPU,
        IntegratedGPU,
        Unknown
    };

    /// @brief Information about a physical device
    struct PhysicalDevice
    {
        std::string                  name;
        PhysicalDeviceType           type;
        std::uint32_t                video_ram_bytes;
        std::optional<std::uint32_t> graphics_queue_family;

        [[nodiscard]] constexpr bool
        operator==(const PhysicalDevice &rhs) const noexcept
        {
            return name == rhs.name && type == rhs.type && video_ram_bytes == rhs.video_ram_bytes
                   && graphics_queue_family == rhs.graphics_queue_family;
        }
    };
} // namespace xgpu::data