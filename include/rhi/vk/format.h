#ifndef RHI_VK_FORMAT_H
#define RHI_VK_FORMAT_H

#include "rhi/format.h"
#include "rhi/vk/core.h"

namespace xgpu::vk
{
    [[nodiscard]] auto convertFormat(xgpu::Format format) noexcept -> VkFormat;
} // namespace xgpu::vk

#endif // RHI_VK_FORMAT_H
