#ifndef RHI_PIPELINE_H
#define RHI_PIPELINE_H

#include "rhi/format.h"

#include <cstdint>
#include <span>
#include <variant>

namespace rhi
{
    struct VertexInputDescription
    {
        struct AttributeDescription
        {
            std::uint32_t binding;
            std::uint32_t offset;
            std::uint32_t location;
            Format format;
        };

        struct Binding
        {
            std::uint32_t binding;
            std::uint32_t stride;
        };

        std::span<AttributeDescription> attribute_descriptions;
        std::span<Binding> bindings;
    };
    
    struct OpenPipelineDescription
    {
        VertexInputDescription vertex_input;
    };


    using PipelineDescription = std::variant<OpenPipelineDescription>;
    
    class Pipeline
    {
        public:
        private:
    };
} // namespace rhi

#endif // RHI_PIPELINE_H
