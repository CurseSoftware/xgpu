#ifndef RHI_VK_IMAGE_VIEW_H
#define RHI_VK_IMAGE_VIEW_H

#include "xgpu/vk/core.h"

#include "xgpu/expected.h"
#include "xgpu/error.h"
#include "xgpu/image_view.h"
#include "xgpu/device.h"
#include <cstdint>


namespace xgpu::vk
{
    class ImageView : public IImageView
    {
        // Factory
        public:
            [[nodiscard]] static auto from_open(xgpu::Device&, const ImageViewDescription&) noexcept -> expected<vk::ImageView, Error>;

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
} // namespace xgpu::vk 

#endif // RHI_VK_IMAGE_VIEW_H
