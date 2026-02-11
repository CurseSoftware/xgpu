#include "rhi/format.h"
#include "rhi/vk/format.h"
#include "rhi/vk/core.h"
#include <vulkan/vulkan_core.h>


namespace xgpu::vk
{
    // Largely thanks to NVRHI thanks :)
    // https://github.com/NVIDIA-RTX/NVRHI/blob/main/src/vulkan/vulkan-constants.cpp
    auto convertFormat(xgpu::Format format) noexcept -> VkFormat
    {
        switch (format)
        {
            case Format::Unknown:           return VK_FORMAT_UNDEFINED;
            case Format::R8_UINT:           return VK_FORMAT_R8_UINT;
            case Format::R8_SINT:           return VK_FORMAT_R8_SINT;                  
            case Format::R8_UNORM:          return VK_FORMAT_R8_UNORM;
            case Format::R8_SNORM:          return VK_FORMAT_R8_SNORM;
            case Format::RG8_UINT:          return VK_FORMAT_R8G8_UINT;
            case Format::RG8_SINT:          return VK_FORMAT_R8G8_SINT;
            case Format::RG8_UNORM:         return VK_FORMAT_R8G8_UNORM;
            case Format::RG8_SNORM:         return VK_FORMAT_R8G8_SNORM;
            case Format::R16_UINT:          return VK_FORMAT_R16_UINT;
            case Format::R16_SINT:          return VK_FORMAT_R16_SINT;
            case Format::R16_UNORM:         return VK_FORMAT_R16_UNORM;
            case Format::R16_SNORM:         return VK_FORMAT_R16_SNORM;
            case Format::R16_FLOAT:         return VK_FORMAT_R16_SFLOAT;
            case Format::BGRA4_UNORM:       return VK_FORMAT_A4R4G4B4_UNORM_PACK16;
            case Format::B5G6R5_UNORM:      return VK_FORMAT_B5G6R5_UNORM_PACK16;
            case Format::B5G5R5A1_UNORM:    return VK_FORMAT_B5G5R5A1_UNORM_PACK16;
            case Format::RGBA8_UINT:        return VK_FORMAT_R8G8B8A8_UINT;
            case Format::RGBA8_SINT:        return VK_FORMAT_R8G8B8A8_SINT;
            case Format::RGBA8_UNORM:       return VK_FORMAT_R8G8B8A8_UNORM;
            case Format::RGBA8_SNORM:       return VK_FORMAT_R8G8B8A8_SNORM;
            case Format::BGRA8_UNORM:       return VK_FORMAT_B8G8R8A8_UNORM;
            case Format::BGRX8_UNORM:       return VK_FORMAT_UNDEFINED;
            case Format::SRGBA8_UNORM:      return VK_FORMAT_R8G8B8A8_SRGB;
            case Format::SBGRA8_UNORM:      return VK_FORMAT_B8G8R8A8_SRGB;
            case Format::SBGRX8_UNORM:      return VK_FORMAT_UNDEFINED;
            case Format::R10G10B10A2_UNORM: return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
            case Format::R11G11B10_FLOAT:   return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
            case Format::RG16_UINT:         return VK_FORMAT_R16G16_UINT;
            case Format::RG16_SINT:         return VK_FORMAT_R16G16_SINT;
            case Format::RG16_UNORM:        return VK_FORMAT_R16G16_UNORM;
            case Format::RG16_SNORM:        return VK_FORMAT_R16G16_SNORM;
            case Format::RG16_FLOAT:        return VK_FORMAT_R16G16_SFLOAT;
            case Format::R32_UINT:          return VK_FORMAT_R32_UINT;
            case Format::R32_SINT:          return VK_FORMAT_R32_SINT;
            case Format::R32_FLOAT:         return VK_FORMAT_R32_SFLOAT;
            case Format::RGBA16_UINT:       return VK_FORMAT_R16G16B16A16_UINT;
            case Format::RGBA16_SINT:       return VK_FORMAT_R16G16B16A16_SINT;
            case Format::RGBA16_FLOAT:      return VK_FORMAT_R16G16B16A16_SFLOAT;
            case Format::RGBA16_UNORM:      return VK_FORMAT_R16G16B16A16_UNORM;
            case Format::RGBA16_SNORM:      return VK_FORMAT_R16G16B16A16_SNORM;
            case Format::RG32_UINT:         return VK_FORMAT_R32G32_UINT;
            case Format::RG32_SINT:         return VK_FORMAT_R32G32_SINT;
            case Format::RG32_FLOAT:        return VK_FORMAT_R32G32_SFLOAT;
            case Format::RGB32_UINT:        return VK_FORMAT_R32G32B32_UINT;
            case Format::RGB32_SINT:        return VK_FORMAT_R32G32B32_SINT;
            case Format::RGB32_FLOAT:       return VK_FORMAT_R32G32B32_SFLOAT;
            case Format::RGBA32_UINT:       return VK_FORMAT_R32G32B32A32_UINT;
            case Format::RGBA32_SINT:       return VK_FORMAT_R32G32B32A32_SINT;
            case Format::RGBA32_FLOAT:      return VK_FORMAT_R32G32B32A32_SFLOAT;
            case Format::D16:               return VK_FORMAT_D16_UNORM;
            case Format::D24S8:             return VK_FORMAT_D24_UNORM_S8_UINT;
            case Format::X24G8_UINT:        return VK_FORMAT_D24_UNORM_S8_UINT;
            case Format::D32:               return VK_FORMAT_D32_SFLOAT;
            case Format::D32S8:             return VK_FORMAT_D32_SFLOAT_S8_UINT;
            case Format::X32G8_UINT:        return VK_FORMAT_D32_SFLOAT_S8_UINT;
            case Format::BC1_UNORM:         return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
            case Format::BC1_UNORM_SRGB:    return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
            case Format::BC2_UNORM:         return VK_FORMAT_BC2_UNORM_BLOCK;
            case Format::BC2_UNORM_SRGB:    return VK_FORMAT_BC2_SRGB_BLOCK;
            case Format::BC3_UNORM:         return VK_FORMAT_BC3_UNORM_BLOCK;
            case Format::BC3_UNORM_SRGB:    return VK_FORMAT_BC3_SRGB_BLOCK;
            case Format::BC4_UNORM:         return VK_FORMAT_BC4_UNORM_BLOCK;
            case Format::BC4_SNORM:         return VK_FORMAT_BC4_SNORM_BLOCK;
            case Format::BC5_UNORM:         return VK_FORMAT_BC5_UNORM_BLOCK;
            case Format::BC5_SNORM:         return VK_FORMAT_BC5_SNORM_BLOCK;
            case Format::BC6H_UFLOAT:       return VK_FORMAT_BC6H_UFLOAT_BLOCK;
            case Format::BC6H_SFLOAT:       return VK_FORMAT_BC6H_SFLOAT_BLOCK;
            case Format::BC7_UNORM:         return VK_FORMAT_BC7_UNORM_BLOCK;
            case Format::BC7_UNORM_SRGB:    return VK_FORMAT_BC7_SRGB_BLOCK;
            case Format::Count:             return VK_FORMAT_UNDEFINED;
        }

        return VK_FORMAT_UNDEFINED;
    }
} // namespace xgpu::vk
