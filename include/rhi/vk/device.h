#ifndef RHI_VK_DEVICE_H
#define RHI_VK_DEVICE_H
#include <cstdint>
#include <optional>
#include <vulkan/vulkan_core.h>
#ifdef RHI_COMPILE_VULKAN_BACKEND

#include "rhi/expected.h"
#include "vk/device_context.h"

#include "rhi/device.h"

namespace rhi::vk
{
    class Device : public IDevice
    {
        public:
            static auto create(rhi::vk::DeviceContext ctx) noexcept -> expected<Device, Error>;

            auto destroy() noexcept -> void override {}
        
        private:
            explicit Device() = default;
    };

    auto getComputeFamilyIndex(VkPhysicalDevice physical_device) -> std::optional<std::uint32_t>;
    auto getGraphicsFamilyIndex(VkPhysicalDevice physical_device) -> std::optional<std::uint32_t>;
    auto getTransferFamilyIndex(VkPhysicalDevice physical_device) -> std::optional<std::uint32_t>;
} // namespace rhi::vk

#endif // RHI_COMPILE_VULKAN_BACKEND
#endif // RHI_VK_DEVICE_H
