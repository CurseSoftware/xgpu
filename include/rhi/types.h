#ifndef RHI_TYPES_H
#define RHI_TYPES_H

#include "core/flags.h"
#include <cstdint>

namespace rhi
{
    enum class QueueFamilyIndex
    {
        Graphics,
        Present,
        Compute
    };

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

    enum class ImageUsageFlagBits : std::uint32_t
    {
        TransferSrc            = 0,
        TransferDst            = 1 << 0,
        Sampled                = 1 << 1,
        Storage                = 1 << 2,
        ColorAttachment        = 1 << 3,
        DepthStencilAttachment = 1 << 4,
        InputAttachment        = 1 << 5,
    };

    class ImageUsage : public Flags<std::uint32_t>
    {
        public:
            using Flags::Flags;
            static constexpr auto TransferSrc()            -> ImageUsage { return ImageUsage(static_cast<std::uint32_t>(ImageUsageFlagBits::TransferSrc)); }
            static constexpr auto TransferDst()            -> ImageUsage { return ImageUsage(static_cast<std::uint32_t>(ImageUsageFlagBits::TransferDst)); }
            static constexpr auto Sampled()                -> ImageUsage { return ImageUsage(static_cast<std::uint32_t>(ImageUsageFlagBits::Sampled)); }
            static constexpr auto Storage()                -> ImageUsage { return ImageUsage(static_cast<std::uint32_t>(ImageUsageFlagBits::Storage)); }
            static constexpr auto ColorAttachment()        -> ImageUsage { return ImageUsage(static_cast<std::uint32_t>(ImageUsageFlagBits::ColorAttachment)); }
            static constexpr auto DepthStencilAttachment() -> ImageUsage { return ImageUsage(static_cast<std::uint32_t>(ImageUsageFlagBits::DepthStencilAttachment)); }
            static constexpr auto InputAttachment()        -> ImageUsage { return ImageUsage(static_cast<std::uint32_t>(ImageUsageFlagBits::InputAttachment)); }
    };

    enum class ImageAspectFlagBits : std::uint32_t
    {
        AspectNone = 0,
        Color      = 1 << 0,
        Depth      = 1 << 1,
        Stencil    = 1 << 2,
    };

    class ImageAspectFlags : public Flags<std::uint32_t>
    {
        public:
            using Flags::Flags;
            static constexpr auto Color()      -> ImageAspectFlags { return ImageAspectFlags(static_cast<std::uint32_t>(ImageAspectFlagBits::Color)); }
            static constexpr auto Depth()      -> ImageAspectFlags { return ImageAspectFlags(static_cast<std::uint32_t>(ImageAspectFlagBits::Depth)); }
            static constexpr auto Stencil()    -> ImageAspectFlags { return ImageAspectFlags(static_cast<std::uint32_t>(ImageAspectFlagBits::Stencil)); }
            static constexpr auto AspectNone() -> ImageAspectFlags { return ImageAspectFlags(static_cast<std::uint32_t>(ImageAspectFlagBits::AspectNone)); }
    };

    enum class ImageType
    {
        Type1D,
        Type2D,
        Type3D,
    };

    enum class ImageTiling
    {
        Optimal,
        Linear,
        DrmFormatModifier
    };

    enum class SharingMode
    {
        Exclusive,
        Concurrent
    };
    
    enum class LoadOperation
    {
        Load,
        Clear,
        DontCare,
    };

    enum class StoreOperation
    {
        Store,
        DontCare
    };

    enum class ImageLayout
    {
        Undefined,
        ColorOptimal,
        Present,
        TransferSrc,
        TransferDst,
    };

    enum class SubpassBindPoint
    {
        Graphics,
        Compute,
    };

    enum class AttachmentType : std::uint32_t
    {
        Color,
        DepthStencil,
        ReadOnly,
        ShaderReadOnly
    };
    
    struct Extent2D
    {
        std::uint32_t width;
        std::uint32_t height;
    };

    struct Extent3D
    {
        std::uint32_t width;
        std::uint32_t height;
        std::uint32_t depth { 1 };
    };
} // namespace rhi

#endif // RHI_TYPES_H
