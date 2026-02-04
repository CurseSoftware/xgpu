#include "rhi/pipeline.h"
#include "vk/pipeline.h"
#include <memory>
#include <variant>

namespace rhi
{
    auto Pipeline::create(rhi::Device &p_device, const GraphicsPipelineDescription &p_description) -> expected<Pipeline, Error>
    {
        Pipeline pipeline {};
        if (std::holds_alternative<OpenGraphicsPipelineDescription>(p_description))
        {
            switch (p_device.backend())
            {
                case Backend::Vulkan:
                {
                    auto expected_pipeline = vk::Pipeline::from_open(p_device, p_description);
                    if (!expected_pipeline.has_value())
                    {
                        return unexpected( expected_pipeline.unwrap_error() );
                    }

                    pipeline._handle = std::make_unique<vk::Pipeline>(expected_pipeline.unwrap());
                    return ok(pipeline);
                } break;
                default:
                    return unexpected( Error("Unsupported backend for creating graphics pipeline") );
            }
        }

        return unexpected( Error("Failed to make pipeline") );
    }
} // namespace rhi
