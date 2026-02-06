#include "rhi/image_view.h"
#include "rhi/expected.h"
#include "rhi/vk/image_view.h"
#include <memory>

namespace rhi
{
    auto ImageView::create(Device & device, const ImageViewDescription & description) noexcept -> expected<ImageView, Error>
    {
        ImageView image_view {};

        switch (device.backend())
        {
            case Backend::Vulkan:
                {
#ifdef RHI_COMPILE_VULKAN_BACKEND
                    auto expected_image_view = vk::ImageView::from_open(device, description);
                    if (!expected_image_view.has_value())
                    {
                        return unexpected( expected_image_view.unwrap_error() );
                    }
                    image_view._handle = std::make_unique<vk::ImageView>(expected_image_view.unwrap());
#else
                return unexpected( Error("Cannot create vulkan image view. Vulkan backend is not compiled") );
#endif // RHI_COMPILE_VULKAN_BACKEND
                } break;
            case Backend::Dx12:
                return unexpected( Error("Cannot create Dx12 image view. Dx12 backend is not supported") );
            case Backend::Metal:
                return unexpected( Error("Cannot create Metal image view. Metal backend is not supported") );
        }

        return ok(image_view);
    }
} // namespace rhi
