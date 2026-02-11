#include "rhi/vk/image_view.h"
#include "core/log.h"
#include "rhi/vk/device.h"
#include "types.h"
#include "vk/format.h"
#include "vk/vk_utils.h"
#include <vulkan/vulkan_core.h>

namespace xgpu::vk
{
    auto ImageView::from_open(xgpu::Device &device, const ImageViewDescription &description) noexcept -> expected<vk::ImageView, Error>
    {
        auto* vk_device = dynamic_cast<vk::Device*>(device.handle());
        if (!vk_device)
        {
            return unexpected( Error("Failed to get vk::Device from rhi::Device") );
        }

        ImageView image_view { vk_device->handle() };
        image_view._detail.layer_count = description.array_layers;

        // Create the image
        {
            VkImageCreateInfo image_info {
                .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                .imageType = convertImageType(description.image_type),
                .format = convertFormat(description.format),
                .extent = {
                    .width = description.extent.width,
                    .height = description.extent.height,
                    .depth = description.extent.depth,
                },
                .mipLevels = description.mip_levels,
                .arrayLayers = description.array_layers,
                .samples = convertSampleCount(description.samples),
                .tiling = convertImageTiling(description.tiling),
                .usage = getVulkanImageUsage(description.usage),
                .sharingMode = convertSharingMode(description.sharing_mode),
                .initialLayout = convertImageLayout(description.initial_layout),
            };

            const VkResult result = vkCreateImage(vk_device->handle(), &image_info, nullptr, &image_view._image);
            if (result != VK_SUCCESS)
            {
                return unexpected( Error("vkCreateImage != VK_SUCCESS") );
            }
        }

        // Allocate the image
        {
            VkMemoryRequirements mem_requirements {};
            vkGetImageMemoryRequirements(vk_device->handle(), image_view._image, &mem_requirements);

            auto memory_index = vk_device->getMemoryTypeIndex(mem_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            if (!memory_index.has_value())
            {
                return unexpected( Error("Failed to get memory type index from physical device for vulkan image allocation") );
            }
            
            VkMemoryAllocateInfo alloc_info {
                .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                .allocationSize = mem_requirements.size,
                .memoryTypeIndex = *memory_index
            };

            std::uint32_t offset { 0 };
            vkAllocateMemory(vk_device->handle(), &alloc_info, nullptr, &image_view._memory);
            vkBindImageMemory(vk_device->handle(), image_view._image, image_view._memory, offset);
        }

        // Create the image view
        {
            VkImageViewCreateInfo view_info {
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .image = image_view._image,
                .viewType = description.image_type == ImageType::Type1D 
                    ? VK_IMAGE_VIEW_TYPE_1D
                    : description.image_type == ImageType::Type2D
                        ? VK_IMAGE_VIEW_TYPE_2D
                        : VK_IMAGE_VIEW_TYPE_3D,
                .format = convertFormat(description.format),
                .subresourceRange = {
                    .aspectMask = convertImageAspect(description.aspect),
                    .baseMipLevel = 0,
                    .levelCount = description.mip_levels,
                    .baseArrayLayer = 0,
                    .layerCount = description.array_layers,
                },
                // TODO: add mapping?
            };

            const VkResult result = vkCreateImageView(vk_device->handle(), &view_info, nullptr, &image_view._handle);
            if (result != VK_SUCCESS)
            {
                return unexpected( Error("vkCreateImageView != VK_SUCCESS") );
            }
        }

        return ok(image_view);
    }

    auto ImageView::destroy() noexcept -> void
    {
        log::trace("Destroying vulkan image view...");
        vkDestroyImageView(_device, _handle, nullptr);
        log::trace("Destroyed.");
        log::trace("Destroying vulkan image...");
        vkDestroyImage(_device, _image, nullptr);
        log::trace("Destroyed.");
        log::trace("Freeing vulkan image memory...");
        vkFreeMemory(_device, _memory, nullptr);
        log::trace("Freed.");
    }
} // namespace xgpu::vk
