#pragma once
#include "xgpu/core/core.h"
#ifdef XGPU_COMPILE_VULKAN

#include "vulkan_error.h"

#include <cstdint>
#include <vector>

namespace xgpu::vk
{
    /// @brief Commonly relevant properties for a vulkan physical device
    struct PhysicalDeviceProperties
    {
        VkPhysicalDevice                     device;
        VkPhysicalDeviceProperties           properties;
        VkPhysicalDeviceMemoryProperties     memory_properties;
        std::vector<VkQueueFamilyProperties> queue_family_properties;
    };

    /// @brief Create the vulkan device
    [[nodiscard]] Result create_device(
        VkPhysicalDevice physical_device, const VkDeviceCreateInfo &create_info, VkDevice &device) noexcept;

    /// @brief Get the queue family properties for a physical device
    [[nodiscard]] std::vector<VkQueueFamilyProperties>
    get_device_queue_family_properties(VkPhysicalDevice physical_device) noexcept;

    /// @brief Get the enumeration of available physical devices
    [[nodiscard]] std::vector<PhysicalDeviceProperties>
    enumerate_physical_devices(VkInstance instance) noexcept;

    /// @brief Get the properties of a physical device
    [[nodiscard]] VkPhysicalDeviceProperties
    get_physical_device_properties(VkPhysicalDevice physical_device) noexcept;

    /// @brief Get the memory properties of a physical device
    [[nodiscard]] VkPhysicalDeviceMemoryProperties
    get_physical_device_memory_properties(VkPhysicalDevice physical_device) noexcept;

    /// @brief Get all the properties for a physical device
    [[nodiscard]] PhysicalDeviceProperties
    get_aggregate_device_properties(VkPhysicalDevice physical_device) noexcept;

    /// @brief Get the physical devices queue family index for a given family if it exists
    template <VkQueueFlags QUEUE>
    [[nodiscard]] std::optional<std::uint32_t>
    get_queue_family_index(const PhysicalDeviceProperties &properties) noexcept
    {
        for ( std::size_t i = 0; i < properties.queue_family_properties.size(); ++i ) {
            if ( properties.queue_family_properties[i].queueFlags & QUEUE ) {
                return i;
            }
        }

        return std::nullopt;
    }

    /// @brief Get the physical devices queue family index for a given family if it exists
    template <VkQueueFlags QUEUE>
    [[nodiscard]] std::optional<std::uint32_t>
    get_queue_family_index(VkPhysicalDevice physical_device) noexcept
    {
        PhysicalDeviceProperties properties = get_aggregate_device_properties(physical_device);
        return get_queue_family_index<QUEUE>(properties);
    }
} // namespace xgpu::vk

#endif // XGPU_COMPILE_VULKAN
