#ifndef RHI_DEVICE_H
#define RHI_DEVICE_H

#include "rhi/core.h"
#include "rhi/error.h"
#include "rhi/expected.h"
#include "rhi/instance.h"
#include "rhi/platform.h"
#include "rhi/vk/device_context.h"
#include "rhi/types.h"
#include <memory>
#include <variant>
namespace xgpu
{
    struct DefaultDeviceContext 
    {
        xgpu::Instance& instance;
        Preference graphics_preference { true };
        Preference transfer_preference { false };
        Preference compute_preference { true };
    };

    using DeviceContext = std::variant<
        DefaultDeviceContext

#ifdef RHI_COMPILE_VULKAN_BACKEND
        , vk::DeviceContext
#endif // RHI_COMPILE_VULKAN_BACKEND

#ifdef RHI_COMPILE_DX12_BACKEND
#endif // RHI_COMPILE_DX12_BACKEND

#ifdef RHI_COMPILE_METAL_BACKEND
#endif // RHI_COMPILE_METAL_BACKEND
 
    >;

    class IDevice
    {
        public:
            virtual auto destroy() noexcept -> void = 0;

            [[nodiscard]] virtual auto backend() noexcept -> Backend = 0;

            [[nodiscard]] virtual auto graphics_queue() const noexcept -> std::optional<std::uint32_t> = 0;
            [[nodiscard]] virtual auto compute_queue() const noexcept -> std::optional<std::uint32_t> = 0;
            [[nodiscard]] virtual auto present_queue() const noexcept -> std::optional<std::uint32_t> = 0;
            [[nodiscard]] virtual auto transfer_queue() const noexcept -> std::optional<std::uint32_t> = 0;

            virtual auto submitSingle(QueueFamilyIndex queue, class ICommandBuffer* command_buffer) noexcept -> std::optional<Error> = 0;

            virtual auto waitIdle() const noexcept -> void = 0;
    };

    class [[nodiscard]] Device : public IDevice
    {
        public:
            [[nodiscard]]
            static auto create(const DeviceContext& ctx) noexcept -> expected<Device, Error>;
        
            auto destroy() noexcept -> void override { _handle->destroy(); }

            auto backend() noexcept -> Backend override { return _handle->backend(); }

            [[nodiscard]] auto handle() noexcept -> IDevice* { return _handle.get(); }

            [[nodiscard]] auto graphics_queue() const noexcept -> std::optional<std::uint32_t> override { return _handle->graphics_queue(); }

            [[nodiscard]] auto compute_queue() const noexcept -> std::optional<std::uint32_t> override { return _handle->compute_queue(); }

            [[nodiscard]] auto present_queue() const noexcept -> std::optional<std::uint32_t> override { return _handle->present_queue(); }

            [[nodiscard]] auto transfer_queue() const noexcept -> std::optional<std::uint32_t> override { return _handle->transfer_queue(); }

            auto submitSingle(QueueFamilyIndex queue, class ICommandBuffer* buffer) noexcept -> std::optional<Error> override { return _handle->submitSingle(queue, buffer); }

            auto waitIdle() const noexcept -> void override { _handle->waitIdle(); }

            template <typename T>
            [[nodiscard]] auto get_as() const noexcept -> T* { return dynamic_cast<T*>(_handle.get()); }
        
        private:
            explicit Device() noexcept = default;


        private:
            std::unique_ptr<IDevice> _handle { nullptr };
    };
} // namespace xgpu

#endif // RHI_DEVICE_H
