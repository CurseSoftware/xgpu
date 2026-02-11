#include "rhi/command.h"
#include "rhi/device.h"
#include "rhi/vk/command.h"

#include <memory>

namespace xgpu
{
    auto CommandPool::create(xgpu::Device& device, const CommandPoolDescription description) noexcept -> expected<CommandPool, Error>
    {
        CommandPool pool {};

        switch (device.backend())
        {
            case Backend::Vulkan:
            {
#ifdef RHI_COMPILE_VULKAN_BACKEND
                auto expected_pool = vk::CommandPool::from_open(device, description);
                if (!expected_pool.has_value())
                {
                    return unexpected( expected_pool.unwrap_error() );
                }
                
                pool._handle = std::make_unique<vk::CommandPool>(expected_pool.unwrap());
#else
                return unexpected( Error("Vulkan backend not compiled") );
#endif // RHI_COMPILE_VULKAN_BACKEND
            } break;
            case Backend::Dx12:
            {
                return unexpected( Error("Dx12 command pool creation not supported") );
            } break;
            case Backend::Metal:
            {
                return unexpected( Error("Metal command pool creation not supported") );
            } break;
        }

        return ok(pool);
    }
} // namespace xgpu
