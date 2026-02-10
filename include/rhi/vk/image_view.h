#ifndef RHI_VK_IMAGE_VIEW_H
#define RHI_VK_IMAGE_VIEW_H

#include "rhi/vk/core.h"

#include "rhi/expected.h"
#include "rhi/error.h"
#include "rhi/image_view.h"
#include "rhi/device.h"
#include <cstdint>


namespace rhi::vk
{
    class ImageView : public IImageView
    {
        // Factory
        public:
            [[nodiscard]] static auto from_open(rhi::Device&, const ImageViewDescription&) noexcept -> expected<vk::ImageView, Error>;

        // API
        public:
            auto destroy() noexcept -> void override;

            [[nodiscard]] auto handle() const noexcept -> VkImageView { return _handle; }

            [[nodiscard]] auto layer_count() const noexcept -> std::uint32_t { return _detail.layer_count; }

            [[nodiscard]] auto image() const noexcept -> VkImage { return _image; }

        // Private special members
        private:
            [[nodiscard]] explicit ImageView(VkDevice device) noexcept
                : _device{ device }
            {}

        private:
            VkDevice _device       { VK_NULL_HANDLE };
            VkImage _image         { VK_NULL_HANDLE };
            VkDeviceMemory _memory { VK_NULL_HANDLE };
            VkImageView _handle    { VK_NULL_HANDLE };

            struct
            {
                std::uint32_t layer_count;
            } _detail;
    };
} // namespace rhi::vk 

#endif // RHI_VK_IMAGE_VIEW_H
