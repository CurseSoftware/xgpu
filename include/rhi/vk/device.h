#ifndef RHI_VK_DEVICE_H
#define RHI_VK_DEVICE_H

#ifdef RHI_COMPILE_VULKAN_BACKEND

#include "rhi/device.h"

namespace rhi::vk
{
    struct device_context : rhi::device_context
    {
    };

    class device
    {
        public:
            static auto create(device_context& ctx) noexcept -> device;
        private:
            explicit device() = default;
    };
} // namespace rhi::vk

#endif // RHI_COMPILE_VULKAN_BACKEND
#endif // RHI_VK_DEVICE_H
