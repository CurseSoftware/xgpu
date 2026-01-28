#ifndef RHI_VK_DEVICE_CONTEXT_H
#define RHI_VK_DEVICE_CONTEXT_H
#ifdef RHI_COMPILE_VULKAN_BACKEND

#include <vulkan/vulkan_core.h>
#include <vector>

namespace rhi::vk
{
    struct DeviceContext
    {
        VkPhysicalDevice physical_device;
        std::vector<const char*> extensions;
    };
} // namespace rhi::vk

#endif // RHI_COMPILE_VULKAN_BACKEND
#endif // RHI_VK_DEVICE_CONTEXT_H
