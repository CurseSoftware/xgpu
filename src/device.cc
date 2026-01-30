#include "rhi/device.h"
#include "expected.h"
#include "platform.h"
#include <variant>

namespace rhi
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
} // namespace rhi
