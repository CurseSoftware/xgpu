#ifndef RHI_TYPES_H
#define RHI_TYPES_H

#include <cstdint>

namespace rhi
{
    enum class Topology : std::uint32_t
    {
        PointList                  = 0,
        LineList                   = 1,
        LineStrip                  = 2,
        TriangleList               = 3,
        TriangleStrip              = 4,
        TriangleFan                = 5,
        LineListWithAdjacency      = 6,
        LineStripWithAdjacency     = 7,
        TriangleListWithAdjacency  = 8,
        TriangleStripWithAdjacency = 9,
        PatchList                  = 10
    };

    enum class PolygonMode : std::uint32_t
    {
        Fill       = 0,
        Line       = 1,
        Point      = 2,
        FillRectNv = 1000153000
    };

    enum class CullMode
    {
        CullNone,
        Front,
        Back,
        FrontAndBack
    };

    enum class FrontFace
    {
        CounterClockwise,
        Clockwise,
    };

    enum class BlendOperator
    {
        Add,
        Subtract,
        ReverseSubtract,
        Min,
        Max,

        // TODO: add the Ext ones (from vulkan)
        Ext,
    };

    enum class BlendFactor
    {
        Zero,
        One,
        SrcColor,
        OneMinuxSrcColor,
        DstColor,
        OneMinusDstColor,
        SrcAlpha,
        OneMinusSrcAlpha,
        DstAlpha,
        OneMinusDstAlpha,
        ConstantColor,
        OneMinusConstantColor,
        ConstantAlpha,
        OneMinusConstantAlpha,
        SrcAlphaSaturate,
        Src1Color,
        OneMinusSrc1Color,
        Src1Alpha,
        OneMinusSrc1Alpha
    };

    enum class ColorComponentFlags
    {
        FlagsNone,
        Red,
        Green,
        Blue,
        Alpha,
        All
    };

    enum class PipelineDynamicState
    {
        Scissor,
        Viewport,
        CullMode,
        DepthBias,
        LineWidth,
        FrontFace,
        DepthBounds,
        StencilOperator
    };

    enum class LogicOperator
    {
        Clear,
        And,
        AndReverse,
        Copy,
        AndInverted,
        NoOp,
        Xor,
        Or,
        Nor,
        Equivalent,
        Invert,
        OrReverse,
        CopyInverted,
        OrInverted,
        Nand,
        Set
    };

    enum class StencilOperator : std::uint32_t
    {
        Keep               = 0,
        Zero               = 1,
        Replace            = 2,
        IncrementAndClamp  = 3,
        DecrementAndClamp = 4,
        Invert             = 5,
        IncrementAndWrap = 6,
        DecrementAndWrap  = 7,
    };

    enum class CompareOperator : std::uint32_t
    {
        Never          = 0,
        Less           = 1,
        Equal          = 2,
        LessOrEqual    = 3,
        Greater        = 4,
        NotEqual       = 5,
        GreaterOrEqual = 6,
        OpAlways       = 7
    };

    enum class DescriptorType : std::uint32_t
    {
        Sampler,
        CombinedImageSampler,
        SampledImage,
        StorageImage,
        UniformTexelBuffer,
        StorageTexelBuffer,
        UniformBuffer,
        StorageBuffer,
        UniformBufferDynamic,
        StorageBufferDynamic,
        InputAttachment,
        InlineUniformBlock,
        AccelerationStructureKHR,
        AccelerationStructureNV,
        MutableExt,
        InlineUniformBlockExt,
        MutableValve,
    };

    enum class SampleCount
    {
        One,
        Two,
        Four,
        Eight,
        Sixteen,
        ThirtyTwo,
        SixtyFour
    };
} // namespace rhi

#endif // RHI_TYPES_H
