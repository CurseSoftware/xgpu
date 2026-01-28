#ifndef RHI_DEVICE_H
#define RHI_DEVICE_H

#include "rhi/error.h"
#include "rhi/expected.h"
#include "rhi/instance.h"
#include "rhi/vk/device_context.h"
#include <memory>
#include <variant>
namespace rhi
{
    struct DefaultDeviceContext 
    {
        Instance instance;
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
        virtual auto destroy() noexcept -> void = 0;
    };

    class [[nodiscard]] Device : public IDevice
    {
        public:
            [[nodiscard]]
            static auto create(const DeviceContext& ctx) noexcept -> expected<Device, Error>;
        
        private:
            explicit Device() noexcept;

        private:
            std::unique_ptr<IDevice> _handle { nullptr };
    };
} // namespace rhi

#endif // RHI_DEVICE_H
