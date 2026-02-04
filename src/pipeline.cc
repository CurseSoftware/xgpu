#include "rhi/pipeline.h"
#include <variant>

namespace rhi
{
    auto Pipeline::create(rhi::Device &p_device, const GraphicsPipelineDescription &p_description) -> expected<Pipeline, Error>
    {
        if (std::holds_alternative<OpenGraphicsPipelineDescription>(p_description))
        {
            switch (p_device.backend())
            {
                case Backend::Vulkan:

                default:
                    return unexpected( Error("Unsupported backend for creating graphics pipeline") );
            }
        }

        return unexpected( Error("Failed to make pipeline") );
    }
} // namespace rhi
