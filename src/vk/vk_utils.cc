#include "vk/vk_utils.h"
#include "types.h"
#include <cstdint>
#include <vulkan/vulkan_core.h>

namespace rhi::vk
{

    auto convertPipelineStage(PipelineStage stage) -> VkPipelineStageFlags
    {
        switch (stage)
        {
            case PipelineStage::Top:
                return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            case PipelineStage::Bottom:
                return VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
            case PipelineStage::Vertex:
                return VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
            case PipelineStage::Fragment:
                return VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            case PipelineStage::Transfer:
                return VK_PIPELINE_STAGE_TRANSFER_BIT;
            case PipelineStage::Compute:
                return VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
            case PipelineStage::AllGraphics:
                return VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
        }

        return VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
    }

    auto convertSharingMode(SharingMode mode) -> VkSharingMode
    {
        switch (mode)
        {
            case SharingMode::Exclusive:
                return VK_SHARING_MODE_EXCLUSIVE;
            case SharingMode::Concurrent:
                return VK_SHARING_MODE_CONCURRENT;
        }

        return VK_SHARING_MODE_MAX_ENUM;
    }

    auto convertImageTiling(ImageTiling tiling) -> VkImageTiling
    {
        switch (tiling)
        {
            case ImageTiling::Linear:
                return VK_IMAGE_TILING_LINEAR;
            case ImageTiling::Optimal:
                return VK_IMAGE_TILING_OPTIMAL;
            case ImageTiling::DrmFormatModifier:
                return VK_IMAGE_TILING_DRM_FORMAT_MODIFIER_EXT;
        }

        return VK_IMAGE_TILING_MAX_ENUM;
    }

    auto converBufferUsageFlagBits(BufferUsageFlagBits bits) -> VkBufferUsageFlagBits
    {
        switch (bits)
        {
            case BufferUsageFlagBits::Vertex:
                return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            case BufferUsageFlagBits::Index:
                return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            case BufferUsageFlagBits::CopySrc:
                return VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            case BufferUsageFlagBits::CopyDst:
                return VK_BUFFER_USAGE_TRANSFER_DST_BIT;
            case BufferUsageFlagBits::Storage:
                return VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
            case BufferUsageFlagBits::Uniform:
                return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            case BufferUsageFlagBits::Indirect:
                return VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
            
            default:
                return VK_BUFFER_USAGE_FLAG_BITS_MAX_ENUM;
        }

        return VK_BUFFER_USAGE_FLAG_BITS_MAX_ENUM;
    }
    
    auto convertBufferUsage(BufferUsageFlags flags) -> VkBufferUsageFlags
    {
        VkBufferUsageFlags result { 0 };
        std::uint32_t bits = flags.get();

        if (bits & BufferUsageFlags::Vertex().get())
        {
            result |= converBufferUsageFlagBits(BufferUsageFlagBits::Vertex);
        }

        if (bits & BufferUsageFlags::Index().get())
        {
            result |= converBufferUsageFlagBits(BufferUsageFlagBits::Index);
        }

        if (bits & BufferUsageFlags::CopySrc().get())
        {
            result |= converBufferUsageFlagBits(BufferUsageFlagBits::CopySrc);
        }

        if (bits & BufferUsageFlags::CopyDst().get())
        {
            result |= converBufferUsageFlagBits(BufferUsageFlagBits::CopyDst);
        }

        if (bits & BufferUsageFlags::Storage().get())
        {
            result |= converBufferUsageFlagBits(BufferUsageFlagBits::Storage);
        }

        if (bits & BufferUsageFlags::Storage().get())
        {
            result |= converBufferUsageFlagBits(BufferUsageFlagBits::Storage);
        }

        if (bits & BufferUsageFlags::Indirect().get())
        {
            result |= converBufferUsageFlagBits(BufferUsageFlagBits::Indirect);
        }

        return result;
    }
    
    auto convertImageAspect(ImageAspectFlags aspect) -> VkImageAspectFlags
    {
        VkImageAspectFlags result { 0 };
        std::uint32_t bits = aspect.get();

        if (bits & ImageAspectFlags::AspectNone().get())
        {
            result |= VK_IMAGE_ASPECT_NONE_KHR;
        }

        if (bits & ImageAspectFlags::Color().get())
        {
            result |= VK_IMAGE_ASPECT_COLOR_BIT;
        }

        if (bits & ImageAspectFlags::Depth().get())
        {
            result |= VK_IMAGE_ASPECT_DEPTH_BIT;
        }

        if (bits & ImageAspectFlags::Stencil().get())
        {
            result |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }

        return result;
    }

    auto getVulkanImageUsage(ImageUsage usage) -> VkImageUsageFlags
    {
        VkImageUsageFlags result { 0 };
        std::uint32_t bits = usage.get();

        if (bits & ImageUsage::Sampled().get())
        {
            result |= VK_IMAGE_USAGE_SAMPLED_BIT;
        }

        if (bits & ImageUsage::Storage().get())
        {
            result |= VK_IMAGE_USAGE_STORAGE_BIT;
        }

        if (bits & ImageUsage::TransferDst().get())
        {
            result |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        }

        if (bits & ImageUsage::TransferSrc().get())
        {
            result |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        }

        if (bits & ImageUsage::ColorAttachment().get())
        {
            result |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        }

        if (bits & ImageUsage::InputAttachment().get())
        {
            result |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
        }

        if (bits & ImageUsage::DepthStencilAttachment().get())
        {
            result |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        }

        return result;
    }

    auto convertImageType(ImageType image_type) -> VkImageType
    {
        switch (image_type)
        {
            case ImageType::Type1D:
                return VK_IMAGE_TYPE_1D;
            case ImageType::Type2D:
                return VK_IMAGE_TYPE_2D;
            case ImageType::Type3D:
                return VK_IMAGE_TYPE_3D;
        }

        return VK_IMAGE_TYPE_2D;
    }
    
    auto convertImageUsage(ImageUsageFlagBits usage) -> VkImageUsageFlags
    {
        switch (usage)
        {
            case ImageUsageFlagBits::Sampled:
                return VK_IMAGE_USAGE_SAMPLED_BIT;
            case ImageUsageFlagBits::TransferSrc:
                return VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
            case ImageUsageFlagBits::TransferDst:
                return VK_IMAGE_USAGE_TRANSFER_DST_BIT;
            case ImageUsageFlagBits::Storage:
                return VK_IMAGE_USAGE_STORAGE_BIT;
            case ImageUsageFlagBits::DepthStencilAttachment:
                return VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            case ImageUsageFlagBits::InputAttachment:
                return VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
            case ImageUsageFlagBits::ColorAttachment:
                return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        }
        
        return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    }

    auto convertTopology(Topology topology) -> VkPrimitiveTopology
    {
        switch (topology)
        {
            case Topology::PointList:
                return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
            case Topology::LineList:
                return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
            case Topology::LineStrip:
                return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
            case Topology::TriangleList:
                return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            case Topology::TriangleStrip:
                return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
            case Topology::TriangleFan:
                return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
            case Topology::LineListWithAdjacency:
                return VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
            case Topology::LineStripWithAdjacency:
                return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
            case Topology::TriangleListWithAdjacency:
                return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
            case Topology::TriangleStripWithAdjacency:
                return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
            case Topology::PatchList:
                return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
        }

        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    }
    
    auto convertPolygonMode(PolygonMode polygon_mode) -> VkPolygonMode
    {
        switch (polygon_mode)
        {
            case PolygonMode::Fill:
                return VK_POLYGON_MODE_FILL;
            case PolygonMode::Line:
                return VK_POLYGON_MODE_LINE;
            case PolygonMode::Point:
                return VK_POLYGON_MODE_POINT;
            case PolygonMode::FillRectNv:
                return VK_POLYGON_MODE_FILL_RECTANGLE_NV;
        }

        return VK_POLYGON_MODE_FILL;
    }

    auto convertCullMode(CullMode cull_mode) -> VkCullModeFlagBits
    {
        switch (cull_mode)
        {
            case CullMode::CullNone:
                return VK_CULL_MODE_NONE;
            case CullMode::Back:
                return VK_CULL_MODE_BACK_BIT;
            case CullMode::Front:
                return VK_CULL_MODE_FRONT_BIT;
            case CullMode::FrontAndBack:
                return VK_CULL_MODE_FRONT_AND_BACK;
        }

        return VK_CULL_MODE_NONE;
    }

    auto convertFrontFace(FrontFace front_face) -> VkFrontFace
    {
        switch (front_face)
        {
            case FrontFace::CounterClockwise:
                return VK_FRONT_FACE_COUNTER_CLOCKWISE;
            case FrontFace::Clockwise:
                return VK_FRONT_FACE_CLOCKWISE;
        }
        
        return VK_FRONT_FACE_COUNTER_CLOCKWISE;
    }

    auto convertBlendOperator(BlendOperator blend_op) -> VkBlendOp
    {
        switch (blend_op)
        {
            case BlendOperator::Add:
                return VK_BLEND_OP_ADD;
            case BlendOperator::Subtract:
                return VK_BLEND_OP_SUBTRACT;
            case BlendOperator::ReverseSubtract:
                return VK_BLEND_OP_REVERSE_SUBTRACT;
            case BlendOperator::Min:
                return VK_BLEND_OP_MIN;
            case BlendOperator::Max:
                return VK_BLEND_OP_MAX;
            default:
                return VK_BLEND_OP_ADD;
        }

        return VK_BLEND_OP_ADD;
    }

    auto convertBlendFactor(BlendFactor blend_factor) -> VkBlendFactor
    {
        switch (blend_factor)
        {
            case BlendFactor::Zero:
                return VK_BLEND_FACTOR_ZERO;
            case BlendFactor::One:
                return VK_BLEND_FACTOR_ONE;
            case BlendFactor::SrcColor:
                return VK_BLEND_FACTOR_SRC_COLOR;
            case BlendFactor::OneMinuxSrcColor:
                return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
            case BlendFactor::DstColor:
                return VK_BLEND_FACTOR_DST_COLOR;
            case BlendFactor::OneMinusDstColor:
                return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
            case BlendFactor::SrcAlpha:
                return VK_BLEND_FACTOR_SRC_ALPHA;
            case BlendFactor::OneMinusSrcAlpha:
                return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            case BlendFactor::DstAlpha:
                return VK_BLEND_FACTOR_DST_ALPHA;
            case BlendFactor::OneMinusDstAlpha:
                return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
            case BlendFactor::ConstantColor:
                return VK_BLEND_FACTOR_CONSTANT_COLOR;
            case BlendFactor::OneMinusConstantColor:
                return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
            case BlendFactor::ConstantAlpha:
                return VK_BLEND_FACTOR_CONSTANT_ALPHA;
            case BlendFactor::OneMinusConstantAlpha:
                return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
            case BlendFactor::SrcAlphaSaturate:
                return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
            case BlendFactor::Src1Color:
                return VK_BLEND_FACTOR_SRC1_COLOR;
            case BlendFactor::OneMinusSrc1Color:
                return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
            case BlendFactor::Src1Alpha:
                return VK_BLEND_FACTOR_SRC1_ALPHA;
            case BlendFactor::OneMinusSrc1Alpha:
                return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
        }

        return VK_BLEND_FACTOR_ZERO;
    }

    auto convertColorComponentFlags(ColorComponentFlags color_component) -> VkColorComponentFlags
    {
        switch (color_component)
        {
            case ColorComponentFlags::FlagsNone:
                return 0;
            case ColorComponentFlags::Red:
                return VK_COLOR_COMPONENT_R_BIT;
            case ColorComponentFlags::Green:
                return VK_COLOR_COMPONENT_G_BIT;
            case ColorComponentFlags::Blue:
                return VK_COLOR_COMPONENT_B_BIT;
            case ColorComponentFlags::Alpha:
                return VK_COLOR_COMPONENT_A_BIT;
            case ColorComponentFlags::All:
                return VK_COLOR_COMPONENT_A_BIT
                    | VK_COLOR_COMPONENT_R_BIT
                    | VK_COLOR_COMPONENT_G_BIT
                    | VK_COLOR_COMPONENT_B_BIT
                    ;
        }

        return 0;
    }

    auto convertDynamicState(PipelineDynamicState dynamic_state) -> VkDynamicState
    {
        switch (dynamic_state)
        {
            case PipelineDynamicState::Viewport:
                return VK_DYNAMIC_STATE_VIEWPORT;
            case PipelineDynamicState::Scissor:
                return VK_DYNAMIC_STATE_SCISSOR;
            case PipelineDynamicState::DepthBias:
                return VK_DYNAMIC_STATE_DEPTH_BIAS;
            case PipelineDynamicState::CullMode:
                return VK_DYNAMIC_STATE_CULL_MODE;
            case PipelineDynamicState::LineWidth:
                return VK_DYNAMIC_STATE_LINE_WIDTH;
            case PipelineDynamicState::FrontFace:
                return VK_DYNAMIC_STATE_FRONT_FACE;
            case PipelineDynamicState::DepthBounds:
                return VK_DYNAMIC_STATE_DEPTH_BOUNDS;
            case PipelineDynamicState::StencilOperator:
                return VK_DYNAMIC_STATE_STENCIL_OP;
        }
    }

    auto convertLogicOperator(LogicOperator logic_op) -> VkLogicOp
    {
        switch (logic_op)
        {
            case LogicOperator::Clear:
                return VK_LOGIC_OP_CLEAR;
            case LogicOperator::And:
                return VK_LOGIC_OP_AND;
            case LogicOperator::AndReverse:
                return VK_LOGIC_OP_AND_REVERSE;
            case LogicOperator::Copy:
                return VK_LOGIC_OP_COPY;
            case LogicOperator::AndInverted:
                return VK_LOGIC_OP_AND_INVERTED;
            case LogicOperator::NoOp:
                return VK_LOGIC_OP_NO_OP;
            case LogicOperator::Xor:
                return VK_LOGIC_OP_XOR;
            case LogicOperator::Or:
                return VK_LOGIC_OP_OR;
            case LogicOperator::Nor:
                return VK_LOGIC_OP_NOR;
            case LogicOperator::Equivalent:
                return VK_LOGIC_OP_EQUIVALENT;
            case LogicOperator::Invert:
                return VK_LOGIC_OP_INVERT;
            case LogicOperator::OrReverse:
                return VK_LOGIC_OP_OR_REVERSE;
            case LogicOperator::CopyInverted:
                return VK_LOGIC_OP_COPY_INVERTED;
            case LogicOperator::OrInverted:
                return VK_LOGIC_OP_OR_INVERTED;
            case LogicOperator::Nand:
                return VK_LOGIC_OP_NAND;
            case LogicOperator::Set:
                return VK_LOGIC_OP_SET;
        }

        return VK_LOGIC_OP_NO_OP;
    }
    
    auto convertStencilOperator(StencilOperator stencil_op) -> VkStencilOp
    {
        switch (stencil_op)
        {
            case StencilOperator::Keep:
                return VK_STENCIL_OP_KEEP;
            case StencilOperator::Zero:
                return VK_STENCIL_OP_ZERO;
            case StencilOperator::Replace:
                return VK_STENCIL_OP_REPLACE;
            case StencilOperator::IncrementAndClamp:
                return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
            case StencilOperator::IncrementAndWrap:
                return VK_STENCIL_OP_INCREMENT_AND_WRAP;
            case StencilOperator::DecrementAndClamp:
                return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
            case StencilOperator::DecrementAndWrap:
                return VK_STENCIL_OP_DECREMENT_AND_WRAP;
            case StencilOperator::Invert:
                return VK_STENCIL_OP_INVERT;
        }

        return VK_STENCIL_OP_KEEP;
    }
    
    auto convertCompareOperator(CompareOperator compare) -> VkCompareOp
    {
        switch (compare)
        {
            case CompareOperator::Never:
                return VK_COMPARE_OP_NEVER;
            case CompareOperator::Less:
                return VK_COMPARE_OP_LESS;
            case CompareOperator::Equal:
                return VK_COMPARE_OP_EQUAL;
            case CompareOperator::Greater:
                return VK_COMPARE_OP_GREATER;
            case CompareOperator::NotEqual:
                return VK_COMPARE_OP_NOT_EQUAL;
            case CompareOperator::GreaterOrEqual:
                return VK_COMPARE_OP_GREATER_OR_EQUAL;
            case CompareOperator::LessOrEqual:
                return VK_COMPARE_OP_LESS_OR_EQUAL;
            case CompareOperator::OpAlways:
                return VK_COMPARE_OP_ALWAYS;
        }

        return VK_COMPARE_OP_NEVER;
    }

    auto convertDescriptorType(DescriptorType descriptor_type) -> VkDescriptorType
    {
        switch (descriptor_type)
        {
            case DescriptorType::Sampler:
                return VK_DESCRIPTOR_TYPE_SAMPLER;
            case DescriptorType::CombinedImageSampler:
                return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            case DescriptorType::SampledImage:
                return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
            case DescriptorType::StorageImage:
                return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
            case DescriptorType::UniformTexelBuffer:
                return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
            case DescriptorType::StorageTexelBuffer:
                return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
            case DescriptorType::UniformBuffer:
                return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            case DescriptorType::StorageBuffer:
                return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            case DescriptorType::UniformBufferDynamic:
                return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
            case DescriptorType::StorageBufferDynamic:
                return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
            case DescriptorType::InputAttachment:
                return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
            case DescriptorType::InlineUniformBlock:
                return VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK;
            case DescriptorType::InlineUniformBlockExt:
                return VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT;
            case DescriptorType::AccelerationStructureNV:
                return VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV;
            case DescriptorType::AccelerationStructureKHR:
                return VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
            case DescriptorType::MutableExt:
                return VK_DESCRIPTOR_TYPE_MUTABLE_VALVE;
            case DescriptorType::MutableValve:
                return VK_DESCRIPTOR_TYPE_MUTABLE_VALVE;
        }

        return VK_DESCRIPTOR_TYPE_MAX_ENUM;
    }
    
    auto convertSampleCount(SampleCount sample_count) -> VkSampleCountFlagBits
    {
        switch (sample_count)
        {
            case SampleCount::One:
                return VK_SAMPLE_COUNT_1_BIT;
            case SampleCount::Two:
                return VK_SAMPLE_COUNT_2_BIT;
            case SampleCount::Four:
                return VK_SAMPLE_COUNT_4_BIT;
            case SampleCount::Eight:
                return VK_SAMPLE_COUNT_8_BIT;
            case SampleCount::Sixteen:
                return VK_SAMPLE_COUNT_16_BIT;
            case SampleCount::ThirtyTwo:
                return VK_SAMPLE_COUNT_32_BIT;
            case SampleCount::SixtyFour:
                return VK_SAMPLE_COUNT_64_BIT;
        }

        return VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;
    }
    
    auto getVulkanAttachmentLoadOp(LoadOperation op) -> VkAttachmentLoadOp
    {
        switch (op)
        {
            case rhi::LoadOperation::Load: return VK_ATTACHMENT_LOAD_OP_LOAD;
            case rhi::LoadOperation::Clear: return VK_ATTACHMENT_LOAD_OP_CLEAR;
            case rhi::LoadOperation::DontCare: return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        }

        return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    }
    
    auto getVulkanAttachmentStoreOp(StoreOperation op) -> VkAttachmentStoreOp
    {
        switch (op)
        {
            case rhi::StoreOperation::Store: return VK_ATTACHMENT_STORE_OP_STORE;
            case rhi::StoreOperation::DontCare: return VK_ATTACHMENT_STORE_OP_DONT_CARE;
        }
        
        return VK_ATTACHMENT_STORE_OP_DONT_CARE;
    }

    auto convertImageLayout(ImageLayout layout) -> VkImageLayout
    {
        switch (layout)
        {
            case ImageLayout::Present: return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            case ImageLayout::Undefined: return VK_IMAGE_LAYOUT_UNDEFINED;
            case ImageLayout::ColorOptimal: return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            case ImageLayout::TransferSrc: return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            case ImageLayout::TransferDst: return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        }
        
        return VK_IMAGE_LAYOUT_UNDEFINED;
    }
    
    auto getVulkanAttachmentLayout(ImageLayout layout) -> VkImageLayout
    {
        return convertImageLayout(layout);
    }
    
    auto getVulkanAttachmentType(AttachmentType type) -> VkImageLayout
    {
        switch (type)
        {
            case AttachmentType::Color: return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            case AttachmentType::DepthStencil: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            case AttachmentType::ReadOnly: return VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
            case AttachmentType::ShaderReadOnly: return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        }

        return VK_IMAGE_LAYOUT_UNDEFINED;
    }
    
    auto getVulkanPipelineBindPoint(SubpassBindPoint bind_point) -> VkPipelineBindPoint
    {
        switch (bind_point)
        {
            case SubpassBindPoint::Graphics: return VK_PIPELINE_BIND_POINT_GRAPHICS;
            case SubpassBindPoint::Compute: return VK_PIPELINE_BIND_POINT_COMPUTE;
        }

        // Default to graphics. We should never get here anyway
        return VK_PIPELINE_BIND_POINT_GRAPHICS;
    }

    auto getVulkanPipelineBindPoint(PipelineBindPoint bind_point) -> VkPipelineBindPoint
    {
        switch (bind_point)
        {
            case PipelineBindPoint::Graphics: return VK_PIPELINE_BIND_POINT_GRAPHICS;
            case PipelineBindPoint::Compute: return VK_PIPELINE_BIND_POINT_COMPUTE;
        }

        // Default to graphics. We should never get here anyway
        return VK_PIPELINE_BIND_POINT_GRAPHICS;
    }
} // namespace rhi::vk
