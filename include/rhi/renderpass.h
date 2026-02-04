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

    enum class AttachmentType : std::uint32_t
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

    class IRenderpass 
    {
        public:
            virtual auto destroy() noexcept -> void = 0;
    };

    class Renderpass : public IRenderpass
    {
        public:
            // Create a renderpass from the 
            [[nodiscard]] static auto create(
                Device& device,
                std::initializer_list<OpenAttachmentDescription> attachments,
                std::initializer_list<OpenSubpassDescription> subpasses
            ) noexcept -> expected<Renderpass, Error>;

        // API
        public:
            auto destroy() noexcept -> void override { _handle->destroy(); }

        private:
            [[nodiscard]] explicit Renderpass() noexcept {}

        private:
            std::unique_ptr<IRenderpass> _handle { nullptr };
            
    };
} // namespace rhi

#endif // RHI_RENDER_PASS_H
