#pragma once
#include "xgpu/native/native.h"

namespace xgpu::vk
{
    /// @brief Get a string version of the VkResult
    std::string_view to_string(const VkResult result) noexcept;
} // namespace xgpu::vk