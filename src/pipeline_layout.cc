#include "rhi/expected.h"
#include "rhi/pipeline_layout.h"
#include "rhi/vk/pipeline_layout.h"

namespace rhi
{
    auto PipelineLayout::create(Device& device, const PipelineLayoutDescription& description) noexcept -> expected<PipelineLayout, Error>
    {
        PipelineLayout layout {};
        switch (device.backend())
        {
            case Backend::Vulkan:
#ifdef RHI_COMPILE_VULKAN_BACKEND
            {
                auto expected_layout = vk::PipelineLayout::from_open(device, description);
                if (!expected_layout.has_value())
                {
                    return unexpected( expected_layout.unwrap_error() );
                }

                layout._handle = std::make_unique<vk::PipelineLayout>(expected_layout.unwrap());
                return ok(layout);
            }
#else
                return unexpected( Error("Cannot create pipeline layout from vulkan device because Vulkan backend is not compiled") );
#endif // RHI_COMPILE_VULKAN_BACKEND
            default:
                return unexpected( Error("Cannot create pipeline layout on unsupported platform.") );
        }

        return unexpected( Error("Failed to create pipeline layout.") );
    }
} // namespace rhi
