#ifndef RHI_DEVICE_H
#define RHI_DEVICE_H

#include "rhi/core.h"
#include "rhi/error.h"
#include "rhi/expected.h"
#include "rhi/instance.h"
#include "rhi/platform.h"
#include "rhi/vk/device_context.h"
#include <memory>
#include <variant>
namespace rhi
{
    struct DefaultDeviceContext 
    {
        rhi::Instance& instance;
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

            virtual auto backend() noexcept -> Backend = 0;

    };

    class [[nodiscard]] Device : public IDevice
    {
        public:
            [[nodiscard]]
            static auto create(const DeviceContext& ctx) noexcept -> expected<Device, Error>;
        
            auto destroy() noexcept -> void override { _handle->destroy(); }

            auto backend() noexcept -> Backend override { return _handle->backend(); }

            [[nodiscard]] auto handle() noexcept -> IDevice* { return _handle.get(); }
        
        private:
            explicit Device() noexcept = default;


        private:
            std::unique_ptr<IDevice> _handle { nullptr };
    };
} // namespace rhi

#endif // RHI_DEVICE_H
