#ifndef RHI_TYPES_H
#define RHI_TYPES_H

#include "core/flags.h"
#include <cstdint>

namespace xgpu
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
        TransferSrc            = 1 << 0,
        TransferDst            = 1 << 1,
        Sampled                = 1 << 2,
        Storage                = 1 << 3,
        ColorAttachment        = 1 << 4,
        DepthStencilAttachment = 1 << 5,
        InputAttachment        = 1 << 6,
    };

    class ImageUsage : public Flags<ImageUsage, std::uint32_t>
    {
        public:
            using Flags::Flags;
            constexpr ImageUsage(ImageUsageFlagBits bits) : Flags(static_cast<std::uint32_t>(bits)) {}

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
        AspectNone = 1 << 0,
        Color      = 1 << 1,
        Depth      = 1 << 2,
        Stencil    = 1 << 3,
    };

    class ImageAspectFlags : public Flags<ImageAspectFlags, std::uint32_t>
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

    enum class PipelineBindPoint
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

    struct Rect2D
    {
        std::int32_t x { 0 };
        std::int32_t y { 0 };
        Extent2D extent {};
    };

    struct ColorRGBA
    {
        float r { 0.0f };
        float g { 0.0f };
        float b { 0.0f };
        float a { 1.0f };
    };

    using ClearColorValue = ColorRGBA;
    struct DepthStencilClearValue
    {
        float depth;
        std::uint32_t stencil;
    };
    
    enum class BufferUsageFlagBits : std::uint32_t
    {
        Vertex       = 1 << 0,
        Index        = 1 << 1,
        Uniform      = 1 << 2,
        Storage      = 1 << 3,
        CopySrc      = 1 << 4,
        CopyDst      = 1 << 5,
        MapRead      = 1 << 6,
        MapWrite     = 1 << 7,
        Indirect     = 1 << 8,
    };

    class BufferUsageFlags : public Flags<BufferUsageFlags, std::uint32_t>
    {
        public:
            using Flags::Flags;
            static constexpr auto Vertex()       -> BufferUsageFlags { return BufferUsageFlags(static_cast<std::uint32_t>(BufferUsageFlagBits::Vertex)); }
            static constexpr auto Index()        -> BufferUsageFlags { return BufferUsageFlags(static_cast<std::uint32_t>(BufferUsageFlagBits::Index)); }
            static constexpr auto Uniform()      -> BufferUsageFlags { return BufferUsageFlags(static_cast<std::uint32_t>(BufferUsageFlagBits::Uniform)); }
            static constexpr auto Storage()      -> BufferUsageFlags { return BufferUsageFlags(static_cast<std::uint32_t>(BufferUsageFlagBits::Storage)); }
            static constexpr auto CopySrc()      -> BufferUsageFlags { return BufferUsageFlags(static_cast<std::uint32_t>(BufferUsageFlagBits::CopySrc)); }
            static constexpr auto CopyDst()      -> BufferUsageFlags { return BufferUsageFlags(static_cast<std::uint32_t>(BufferUsageFlagBits::CopyDst)); }
            static constexpr auto Indirect()     -> BufferUsageFlags { return BufferUsageFlags(static_cast<std::uint32_t>(BufferUsageFlagBits::Indirect)); }
    };

    enum class BufferMapping : std::uint32_t
    {
        MapRead  = 1 << 0,
        MapWrite = 1 << 1,
    };

    enum class PipelineStage
    {
        Top,
        Bottom,
        Vertex,
        Fragment,
        Compute,
        Transfer,
        AllGraphics,

    };
} // namespace xgpu

#endif // RHI_TYPES_H
