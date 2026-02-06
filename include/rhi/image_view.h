#ifndef RHI_IMAGE_VIEW_H
#define RHI_IMAGE_VIEW_H

#include "rhi/types.h"
#include "rhi/format.h"
#include "rhi/device.h"
#include "rhi/error.h"
#include "rhi/expected.h"

#include <cstdint>
#include <memory>

namespace rhi
{
    struct ImageViewDescription
    {
        Extent3D extent            {};
        Format format              { Format::Unknown };
        std::uint32_t mip_levels   { 1 };
        std::uint32_t array_layers { 1 };
        SampleCount samples        { SampleCount::One };
        ImageType image_type       { ImageType::Type2D };
        ImageUsage usage           { 0 };
        ImageTiling tiling         { ImageTiling::Optimal };
        SharingMode sharing_mode   { SharingMode::Exclusive };
        ImageLayout initial_layout { ImageLayout::Undefined };
        ImageAspectFlags aspect    { static_cast<std::uint32_t>(ImageAspectFlagBits::AspectNone) };
    };

    class IImageView
    {
        public:
            virtual auto destroy() noexcept -> void = 0;
    };

    class ImageView : public IImageView
    {
        public:
            [[nodiscard]] static auto create(Device&, const ImageViewDescription&) noexcept -> expected<ImageView, Error>;

        // API
        public:
            auto destroy() noexcept -> void override { _handle->destroy(); }

            [[nodiscard]] auto handle() const noexcept -> IImageView* { return _handle.get(); }

        private:
            ImageView() = default;

        private:
            std::unique_ptr<IImageView> _handle { nullptr };
    };
} // namespace rhi

#endif // RHI_IMAGE_VIEW_H
