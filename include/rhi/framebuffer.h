#ifndef RHI_FRAMEBUFFER_H
#define RHI_FRAMEBUFFER_H

#include "rhi/format.h"

#include <cstdint>
#include <vector>

namespace rhi
{
    struct FramebufferDescription
    {
        std::vector<Format> color_formats {};
        Format depth_format               { Format::Unknown };
        std::uint32_t sample_count        { 1 };
        std::uint32_t sampleQuality       { 0 };
    };
} // namespace rhi

#endif // RHI_FRAMEBUFFER_H
