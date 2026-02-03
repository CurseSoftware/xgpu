#ifndef RHI_RENDER_PASS_H
#define RHI_RENDER_PASS_H

#include "rhi/device.h"
#include "rhi/error.h"
#include "rhi/expected.h"
#include "rhi/format.h"

#include <cstdint>
#include <functional>
#include <initializer_list>
#include <span>
#include <variant>
#include <vector>

namespace rhi
{
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

    enum class AttachmentType
    {
        Color,
        DepthStencil,
        ReadOnly,
        ShaderReadOnly
    };

    struct OpenAttachmentDescription
    {
        Format format;

        LoadOperation load_operation { LoadOperation::DontCare };
        StoreOperation store_operation { StoreOperation::DontCare };
        
        struct
        {
            LoadOperation load_operation { LoadOperation::DontCare };
            StoreOperation store_operation { StoreOperation::DontCare };
        } stencil;

        ImageLayout initial_layout { ImageLayout::Undefined };
        ImageLayout final_layout;

        std::uint32_t sample_count { 1 };
    };

    struct AttachmentReference
    {
        std::uint32_t index;
        AttachmentType type;
    };

    struct OpenSubpassDescription
    {
        SubpassBindPoint bind_point;
        std::vector<AttachmentReference> attachments;
    };

    using AttachmentDescription = std::variant<OpenAttachmentDescription>;

    class Renderpass
    {
        public:
            // Create a renderpass from the 
            [[nodiscard]] static auto create(
                Device& device,
                std::initializer_list<OpenAttachmentDescription> attachments,
                std::initializer_list<OpenSubpassDescription> subpasses
            ) noexcept -> expected<Renderpass, Error>;
        private:
            [[nodiscard]] explicit Renderpass() noexcept {}

            
    };
} // namespace rhi

#endif // RHI_RENDER_PASS_H
