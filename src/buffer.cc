#include "rhi/buffer.h"
#include "vk/buffer.h"

namespace xgpu
{
    auto Buffer::create(Device& device, BufferDescription description) noexcept -> expected<Buffer, Error>
    {
        Buffer buffer {};

        switch (device.backend())
        {
            case Backend::Vulkan:
            {
#ifdef RHI_COMPILE_VULKAN_BACKEND
                auto expected_buffer = vk::Buffer::create(device, description);
                if (!expected_buffer.has_value())
                {
                    return unexpected( expected_buffer.unwrap_error() );
                }
                buffer._handle = std::make_unique<vk::Buffer>(expected_buffer.unwrap());
#else 
#endif // RHI_COMPILE_VULKAN_BACKEND
            } break;
            case Backend::Dx12:
            {
            } break;
            case Backend::Metal:
            {
            } break;
        }

        return ok(buffer);
    }
} // namespace xgpu
