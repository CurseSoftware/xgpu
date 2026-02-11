#include "xgpu/device.h"
#include "xgpu/expected.h"
#include "xgpu/platform.h"
#include "xgpu/vk/device.h"
#include <variant>

namespace xgpu
{
    auto Device::create(const DeviceContext &ctx) noexcept -> expected<Device, Error>
    {
        Device device;

        if (std::holds_alternative<DefaultDeviceContext>(ctx))
        {
            auto default_ctx = std::get<DefaultDeviceContext>(ctx);
            switch (default_ctx.instance.backend())
            {
                case Backend::Vulkan:
                {
                    auto expected_device = vk::Device::create_default(default_ctx);
                    if (!expected_device.has_value())
                    {
                        return unexpected(expected_device.unwrap_error());
                    }
                    
                    device._handle = std::make_unique<vk::Device>(expected_device.unwrap());
                    return ok(device);
                } break;
                case Backend::Dx12:
                    return unexpected( Error("Dx12 backend device creation not supported.") );
                case Backend::Metal:
                    return unexpected( Error("Metal backend device creation not supported.") );
            }
        }

        if constexpr (backendIsAvailable(Backend::Vulkan))
        {
            if (std::holds_alternative<vk::DeviceContext>(ctx))
            {
            }
        }

        return ok(device);
    }
} // namespace xgpu
