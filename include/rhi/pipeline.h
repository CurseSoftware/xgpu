#ifndef RHI_PIPELINE_H
#define RHI_PIPELINE_H

#include "rhi/device.h"
#include "rhi/error.h"
#include "rhi/expected.h"
#include "rhi/format.h"
#include "rhi/renderpass.h"
#include "rhi/shader.h"
#include "rhi/types.h"

#include <cstdint>
#include <initializer_list>
#include <memory>
#include <span>
#include <unordered_map>
#include <variant>

namespace rhi
{
    // Description of a viewport
    struct ViewportDescription
    {
        float width     {};
        float height    {};
        float x         {};
        float y         {};
        float min_depth {};
        float max_depth {};
    };

    // Description of a scissor
    struct ScissorDescription
    {
        struct
        {
            std::int32_t x;
            std::int32_t y;
        } offset {};
        
        struct
        {
            std::uint32_t width;
            std::uint32_t height;
        } extent {};
    };

    // Description of the vertex input state for the graphics pipeline
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

    // Description for the input assembly state for the graphics pipeline
    struct InputAssemblyStateDescription
    {
        Topology topology      { Topology::TriangleList };
        bool primitive_restart { false };
    };

    // Description for the tesselation stage for the graphics pipeline
    struct TesselationStateDescription
    {
        std::uint32_t patch_control_points { 3 };
    };

    // Description of the multisampling state for anti-aliasing for the graphics pipeline
    struct MultisampleStateDescription
    {
        std::uint32_t rasterization_samples { 1 };
        bool shading_enable                 { false };
        float min_sample_shading            { 1.0f };
        const std::uint32_t* sample_mask    { nullptr };
        bool alpha_to_coverage              { false };
        bool alpa_to_one                    { false };
    };

    struct RasterizationStateDescription
    {
        bool depth_clamp                 { false };
        bool rasterizer_discard          { false };
        PolygonMode polygon_mode         {};
        CullMode cull_mode               {};
        FrontFace front_face             {};
        bool depth_bias                  { false };
        float depth_bias_constant_factor { 0.0f };
        float depth_bias_clamp           { 0.0f };
        float depth_bias_slope_factor    { 0.0f };
        float line_width                 { 1.0f };
    };

    struct ColorBlendAttachmentStateDescription
    {
        bool blend_enable                    { false };
        BlendFactor src_color_blend_factor   {};
        BlendFactor dst_color_blend_factor   {};
        BlendOperation color_blend_op        {};
        BlendFactor src_alpha_blend_factor   {};
        BlendFactor dst_alpha_blend_factor   {};
        BlendOperation alpha_blend_op        {};
        ColorComponentFlags color_write_mask {};
    };

    struct ColorBlendStateDescription
    {
        bool logic_op_enable                                        { false };
        LogicOperator logic_op                                      { LogicOperator::Copy };
        std::span<ColorBlendAttachmentStateDescription> attachments {};
        std::array<float, 4> blend_constants                        { 0.0f, 0.0f, 0.0f, 0.0f };
    };

    struct DynamicStateDescription
    {
        std::span<PipelineDynamicState> states {};
    };

    struct StencilOpState
    {
        StencilOperator fail_op       { StencilOperator::Keep };
        StencilOperator pass_op       { StencilOperator::Keep };
        StencilOperator depth_fail_op { StencilOperator::Keep };
        CompareOperator compare_op    { CompareOperator::Never };
        std::uint32_t compare_mask    { 0 };
        std::uint32_t write_mask      { 0 };
        std::uint32_t reference       { 0 };
    };

    struct DepthStencilStateDescription
    {
        bool depth_test                  { false };
        bool depth_write                 { false };
        CompareOperator depth_compare_op { CompareOperator::Less };
        bool depth_bounds_test           { false };
        bool stencil_test                { false };
        StencilOpState front             {};
        StencilOpState back              {};
        float min_depth_bounds           { 0.0f };
        float max_depth_bounds           { 1.0f };
    };

    // Description of the graphics pipeline
    struct OpenGraphicsPipelineDescription
    {
        Renderpass& renderpass;
        bool enable_depth_test                                    { true };

        VertexInputDescription vertex_input                       {};
        InputAssemblyStateDescription input_assembly              {};
        TesselationStateDescription tesselation                   {};
        std::span<ViewportDescription> viewports                  {};
        std::span<ScissorDescription> scissors                    {};
        RasterizationStateDescription rasterization               {};
        MultisampleStateDescription multisample                   {};
        DepthStencilStateDescription depth_stencil                {};
        ColorBlendStateDescription color_blend                    {};
        DynamicStateDescription dynamic_state                     {};
        std::unordered_map<ShaderStageFlags, ShaderModule> stages {};
    };

    using GraphicsPipelineDescription = std::variant<OpenGraphicsPipelineDescription>;

    class IPipeline
    {
        public:
            virtual auto destroy() noexcept -> void = 0;
    };
    
    class Pipeline : public IPipeline
    {
        // Factory
        public:
            [[nodiscard]] static auto create(
                rhi::Device& p_device,
                const GraphicsPipelineDescription& p_description
            ) -> expected<Pipeline, Error>;

        // API
        public:
            auto destroy() noexcept -> void override
            {
                _handle->destroy();
            }
        
        private:
            std::unique_ptr<IPipeline> _handle { nullptr };
    };
} // namespace rhi

#endif // RHI_PIPELINE_H
