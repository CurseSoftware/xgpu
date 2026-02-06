#ifndef RHI_FRAMEBUFFER_H
#define RHI_FRAMEBUFFER_H

#include "rhi/format.h"
#include "rhi/device.h"
#include "rhi/expected.h"
#include "rhi/error.h"
#include "rhi/renderpass.h"
#include "rhi/types.h"

#include <cstdint>
#include <memory>
#include <vector>

namespace rhi
{
    struct FramebufferDescription
    {
        Renderpass& renderpass;
        std::vector<Format> color_formats {};
        Format depth_format               { Format::Unknown };
        SampleCount sample_count          { 1 };
        std::uint32_t sampleQuality       { 0 };
        std::uint32_t width               { 0 };
        std::uint32_t height              { 0 };
    };

    class IFramebuffer
    {
        public:
            virtual auto destroy() noexcept -> void;
    };

    class Framebuffer : public IFramebuffer
    {
        // Factory
        public:
            [[nodiscard]] static auto create(Device& device, const FramebufferDescription& description) noexcept -> expected<Framebuffer, Error>;

        // API
        public:
            auto destroy() noexcept -> void override { _handle->destroy(); }

        // Private special members
        private:
            [[nodiscard]] explicit Framebuffer() noexcept = default;

        // Private fields
        private:
            std::unique_ptr<IFramebuffer> _handle { nullptr };
    };

} // namespace rhi

#endif // RHI_FRAMEBUFFER_H
