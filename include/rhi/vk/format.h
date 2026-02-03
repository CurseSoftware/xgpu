#ifndef RHI_VK_FORMAT_H
#define RHI_VK_FORMAT_H

#include "rhi/format.h"
#include "rhi/vk/core.h"

namespace rhi::vk
{
    [[nodiscard]] auto convertFormat(rhi::Format format) noexcept -> VkFormat;
} // namespace rhi::vk

#endif // RHI_VK_FORMAT_H
