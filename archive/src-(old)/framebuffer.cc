#include "xgpu/framebuffer.h"
#include "xgpu/expected.h"
#include "xgpu/vk/framebuffer.h"
#include <memory>

namespace xgpu
{
    auto Framebuffer::create(Device &device, const FramebufferDescription &description) noexcept -> expected<Framebuffer, Error>
    {
        Framebuffer framebuffer {};

        switch (device.backend())
        {
            case Backend::Vulkan:
                {
#ifdef RHI_COMPILE_VULKAN_BACKEND
                    auto expected_framebuffer = vk::Framebuffer::from_open(device, description);
                    if (!expected_framebuffer.has_value())
                    {
                        return unexpected( expected_framebuffer.unwrap_error() );
                    }

                    framebuffer._handle = std::make_unique<vk::Framebuffer>(expected_framebuffer.unwrap());
#else
                return unexpected( Error("Vulkan Framebuffer creation not supported since Vulkan backend is not compiled") );
#endif // RHI_COMPILE_VULKAN_BACKEND
                } break;
            case Backend::Dx12:
                return unexpected( Error("DiretcX12 Framebuffer creation not supported") );
            case Backend::Metal:
                return unexpected( Error("Metal Framebuffer creation not supported") );
        }

        return ok(framebuffer);
    }
} // namespace xgpu
