#ifndef RHI_PIPELINE_LAYOUT_H
#define RHI_PIPELINE_LAYOUT_H

#include "rhi/expected.h"
#include "rhi/descriptor_set.h"
#include "rhi/error.h"
#include "rhi/shader.h"
#include "rhi/types.h"
#include "rhi/device.h"

#include <memory>
#include <vector>
#include <cstdint>

namespace rhi
{
    struct PushConstantRange
    {
        std::uint32_t offset          { 0 };
        std::uint32_t size            { 0 };
        ShaderStageFlags shader_stage { ShaderStageFlags::Vert };
    };
    
    struct PipelineLayoutDescription
    {
        std::vector<DescriptorSetLayout> descriptor_layouts {};
        std::vector<PushConstantRange> push_constants       {};
    };

    class IPipelineLayout
    {
        public:
            virtual auto destroy() noexcept -> void = 0;
    };

    class PipelineLayout : public IPipelineLayout
    {
        public:
            // Create a new pipeline layout from a PipelineLayoutDescription
            [[nodiscard]] static auto create(Device& device, const PipelineLayoutDescription& description) noexcept -> expected<PipelineLayout, Error>;

            // Trigger the destroy behavior for the pipeline layout
            auto destroy() noexcept -> void override { _handle->destroy(); }

            // Get a pointer to the underlying type of the held handle
            // This is mainly used by the GAPI backends
            auto handle() const noexcept -> IPipelineLayout* { return _handle.get(); }

        private:
            PipelineLayout() = default;

            std::unique_ptr<IPipelineLayout> _handle { nullptr };
    };
} // namespace rhi

#endif // RHI_PIPELINE_LAYOUT_H
