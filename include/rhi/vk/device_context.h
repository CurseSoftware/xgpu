#ifndef RHI_VK_DEVICE_CONTEXT_H
#define RHI_VK_DEVICE_CONTEXT_H
#include <cstdint>
#ifdef RHI_COMPILE_VULKAN_BACKEND

#include <vulkan/vulkan_core.h>
#include <vector>
#include <optional>

namespace rhi::vk
{
    struct DeviceContext
    {
        struct QueueDesc
        {
            std::uint32_t index;
            std::uint32_t count { 1 };
            float priority { 1.0f };
        };

        VkPhysicalDevice physical_device;
        std::vector<const char*> extensions;

        std::optional<QueueDesc> graphics_queue;
        std::optional<QueueDesc> transfer_queue;
        std::optional<QueueDesc> compute_queue;

        VkPhysicalDeviceFeatures physical_device_features {};
    };
} // namespace rhi::vk

#endif // RHI_COMPILE_VULKAN_BACKEND
#endif // RHI_VK_DEVICE_CONTEXT_H
