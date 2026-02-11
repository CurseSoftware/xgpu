#ifndef RHI_FRAMEBUFFER_H
#define RHI_FRAMEBUFFER_H

#include "rhi/device.h"
#include "rhi/expected.h"
#include "rhi/error.h"
#include "rhi/image_view.h"
#include "rhi/renderpass.h"

#include <cstdint>
#include <functional>
#include <memory>

namespace xgpu
{
    struct FramebufferDescription
    {
        Renderpass& renderpass;
        std::span<const std::reference_wrapper<ImageView>> attachments {};
        std::uint32_t width {};
        std::uint32_t height {};
    };

    class IFramebuffer
    {
        public:
            virtual auto destroy() noexcept -> void = 0;
    };

    class Framebuffer : public IFramebuffer
    {
        // Factory
        public:
            [[nodiscard]] static auto create(Device& device, const FramebufferDescription& description) noexcept -> expected<Framebuffer, Error>;

        // API
        public:
            auto destroy() noexcept -> void override { _handle->destroy(); }

            template<typename T>
            [[nodiscard]] auto get_as() noexcept -> T* { return dynamic_cast<T*>(_handle.get()); }

        // Private special members
        private:
            [[nodiscard]] explicit Framebuffer() noexcept = default;

        // Private fields
        private:
            std::unique_ptr<IFramebuffer> _handle { nullptr };
    };

} // namespace xgpu

#endif // RHI_FRAMEBUFFER_H
