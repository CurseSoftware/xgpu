#ifndef RHI_RENDER_PASS_H
#define RHI_RENDER_PASS_H

#include "xgpu/device.h"
#include "xgpu/error.h"
#include "xgpu/expected.h"
#include "xgpu/format.h"
#include "xgpu/types.h"

#include <cstdint>
#include <functional>
#include <initializer_list>
#include <span>
#include <variant>
#include <vector>

namespace xgpu
{

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

            [[nodiscard]] auto handle() -> IRenderpass* { return _handle.get(); }

        private:
            [[nodiscard]] explicit Renderpass() noexcept {}

        private:
            std::unique_ptr<IRenderpass> _handle { nullptr };
            
    };
} // namespace xgpu

#endif // RHI_RENDER_PASS_H
