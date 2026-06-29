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
        /// @brief The name of the device
        std::string        name;

        /// @brief The type of device that this is
        PhysicalDeviceType type;

        /// @brief The amount of video ram in bytes
        std::uint32_t      video_ram_bytes;

        /// @brief The capabilities of this device
        struct Capabilities
        {
            /// @brief `true` if this device supports compute
            bool compute;

            /// @brief `true` if this device supports graphics
            bool graphics;
        } capabilities;

        [[nodiscard]] constexpr bool
        operator==(const PhysicalDevice &rhs) const noexcept
        {
            return name == rhs.name && type == rhs.type && video_ram_bytes == rhs.video_ram_bytes
                   && capabilities.compute == rhs.capabilities.compute
                   && capabilities.graphics == rhs.capabilities.graphics;
        }
    };
} // namespace xgpu::data