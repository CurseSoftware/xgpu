#pragma once
#include "xgpu/core/core.h"

#ifdef XGPU_COMPILE_VULKAN

#ifdef XGPU_PLATFORM_MACOS
#define VK_USE_PLATFORM_METAL_EXT
#define VK_USE_PLATFORM_MACOS_MVK
#endif // XGPU_PLATFORM_MACOS

#include <vulkan/vulkan.h>

#endif // XGPU_COMPILE_VULKAN
