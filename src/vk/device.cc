#include "rhi/vk/device.h"
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vulkan/vulkan_core.h>

namespace rhi::vk
{
    auto getQueueFamilies(VkPhysicalDevice physical_device) -> std::vector<VkQueueFamilyProperties>
    {
        std::vector<VkQueueFamilyProperties> queue_family_properties {};
        std::uint32_t queue_family_count { 0 };

        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);
        queue_family_properties.resize(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_family_properties.data());

        return queue_family_properties;
    }

    auto getComputeFamilyIndex(VkPhysicalDevice physical_device) -> std::optional<std::uint32_t>
    {
        auto queue_families = getQueueFamilies(physical_device);
        
        for (std::size_t i = 0; i < queue_families.size(); i++)
        {
            if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                return i;
            }
        }

        return std::nullopt;
    }

    auto getGraphicsFamilyIndex(VkPhysicalDevice physical_device) -> std::optional<std::uint32_t>
    {
        auto queue_families = getQueueFamilies(physical_device);
        
        for (std::size_t i = 0; i < queue_families.size(); i++)
        {
            if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
            {
                return i;
            }
        }

        return std::nullopt;
    }
    
    auto getTransferFamilyIndex(VkPhysicalDevice physical_device) -> std::optional<std::uint32_t>
    {
        auto queue_families = getQueueFamilies(physical_device);
        
        for (std::size_t i = 0; i < queue_families.size(); i++)
        {
            if (queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
            {
                return i;
            }
        }

        return std::nullopt;
    }
} // namespace rhi::vk
