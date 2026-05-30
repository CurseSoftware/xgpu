#include "device.h"

#include <ranges>

namespace xgpu::vk
{
    Result
    create_device(
        const VkPhysicalDevice    physical_device,
        const VkDeviceCreateInfo &create_info,
        VkDevice                 &device) noexcept
    {
        return Result(vkCreateDevice(physical_device, &create_info, nullptr, &device));
    }

    std::vector<VkQueueFamilyProperties2>
    get_device_queue_family_properties(VkPhysicalDevice physical_device) noexcept
    {
        std::vector<VkQueueFamilyProperties2> queue_family_properties;
        std::uint32_t                         count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties2(physical_device, std::addressof(count), nullptr);
        queue_family_properties.resize(count);
        vkGetPhysicalDeviceQueueFamilyProperties2(
            physical_device, std::addressof(count), queue_family_properties.data());

        return queue_family_properties;
    }

    VkPhysicalDeviceProperties
    get_physical_device_properties(VkPhysicalDevice physical_device) noexcept
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physical_device, std::addressof(properties));
        return properties;
    }

    VkPhysicalDeviceMemoryProperties2
    get_physical_device_memory_properties(VkPhysicalDevice physical_device) noexcept
    {
        VkPhysicalDeviceMemoryProperties2 properties;
        vkGetPhysicalDeviceMemoryProperties2(physical_device, std::addressof(properties));
        return properties;
    }

    [[nodiscard]] PhysicalDeviceProperties
    get_aggregate_device_properties(VkPhysicalDevice physical_device) noexcept
    {
        return PhysicalDeviceProperties{
            .device                  = physical_device,
            .properties              = get_physical_device_properties(physical_device),
            .memory_properties       = get_physical_device_memory_properties(physical_device),
            .queue_family_properties = get_device_queue_family_properties(physical_device),
        };
    }

    std::vector<PhysicalDeviceProperties>
    enumerate_physical_devices(VkInstance instance) noexcept
    {
        std::vector<PhysicalDeviceProperties> devices;
        std::vector<VkPhysicalDevice>         physical_devices;
        std::uint32_t                         count = 0;

        vkEnumeratePhysicalDevices(instance, &count, nullptr);
        physical_devices.resize(count);
        vkEnumeratePhysicalDevices(instance, &count, physical_devices.data());

        std::ranges::for_each(physical_devices, [&devices](VkPhysicalDevice physical_device) {
            devices.push_back(get_aggregate_device_properties(physical_device));
        });

        return devices;
    }
} // namespace xgpu::vk