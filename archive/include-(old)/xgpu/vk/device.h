#ifndef RHI_VK_DEVICE_H
#define RHI_VK_DEVICE_H
#ifdef RHI_COMPILE_VULKAN_BACKEND

#include <cstdint>
#include <optional>

#include "xgpu/expected.h"
#include "xgpu/vk/device_context.h"

#include "xgpu/device.h"

namespace xgpu::vk
{
    // Information portaining to the capabilities and attributes of a vulkan physical device
    struct PhysicalDeviceInfo
    {
        VkPhysicalDevice handle;
        VkPhysicalDeviceFeatures features;
        VkPhysicalDeviceProperties properties;
        std::optional<std::uint32_t> graphics_family_index;
        std::optional<std::uint32_t> transfer_family_index;
        std::optional<std::uint32_t> compute_family_index;
    };

    class Device : public IDevice
    {
        // Info
        public:
            struct Queue
            {
                VkQueue queue;
                std::uint32_t family_index;
                // TODO: think about adding the queue_index for the queue withing the family
                // this API may require rethinking to support this, but it can probably work
            };
        
        // Factory
        public:
            static auto create(const xgpu::vk::DeviceContext& ctx) noexcept -> expected<Device, Error>;

            static auto create_default(const xgpu::DefaultDeviceContext& ctx) noexcept -> expected<Device, Error>;

        // API
        public:
            auto destroy() noexcept -> void override;

            [[nodiscard]] auto backend() noexcept -> Backend override { return Backend::Vulkan; }

            [[nodiscard]] auto graphics_queue() const noexcept -> std::optional<std::uint32_t> override;
            [[nodiscard]] auto compute_queue() const noexcept -> std::optional<std::uint32_t> override;
            [[nodiscard]] auto transfer_queue() const noexcept -> std::optional<std::uint32_t> override;
            [[nodiscard]] auto present_queue() const noexcept -> std::optional<std::uint32_t> override;

            [[nodiscard]] auto handle() const noexcept -> VkDevice { return _handle; }

            [[nodiscard]] auto getMemoryTypeIndex(std::uint32_t type_bits, VkMemoryPropertyFlags flags) const noexcept -> std::optional<std::uint32_t>;

            auto submitSingle(QueueFamilyIndex queue, ICommandBuffer* buffer) noexcept -> std::optional<Error> override;

            auto waitIdle() const noexcept -> void override;
        
        private:
            explicit Device() = default;

        // Private members
        private:
            VkDevice _handle { VK_NULL_HANDLE };
            VkPhysicalDevice _physical_device;
            VkSurfaceKHR _surface { VK_NULL_HANDLE };
            std::optional<Queue> _graphics_queue { std::nullopt };
            std::optional<Queue> _transfer_queue { std::nullopt };
            std::optional<Queue> _compute_queue { std::nullopt };
            std::optional<Queue> _present_queue { std::nullopt };
    };

    // Get a list of all available supported physical devices
    [[nodiscard]] auto getPhysicalDevices(VkInstance instance) noexcept -> std::vector<VkPhysicalDevice>;

    // Get the attributes and information for a particular physical device
    [[nodiscard]] auto getPhysicalDeviceInfo(VkPhysicalDevice physical_device) noexcept -> PhysicalDeviceInfo;

    [[nodiscard]] auto getComputeFamilyIndex(VkPhysicalDevice physical_device) -> std::optional<std::uint32_t>;
    [[nodiscard]] auto getGraphicsFamilyIndex(VkPhysicalDevice physical_device) -> std::optional<std::uint32_t>;
    [[nodiscard]] auto getTransferFamilyIndex(VkPhysicalDevice physical_device) -> std::optional<std::uint32_t>;
    [[nodiscard]] auto getPresentFamilyIndex(VkPhysicalDevice physical_device) -> std::optional<std::uint32_t>;
} // namespace xgpu::vk

#endif // RHI_COMPILE_VULKAN_BACKEND
#endif // RHI_VK_DEVICE_H
