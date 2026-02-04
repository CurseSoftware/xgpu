#ifndef RHI_SHADER_H
#define RHI_SHADER_H

#include "rhi/device.h"

#include <vector>

namespace rhi
{
    enum class ShaderStageFlags
    {
        Vert,
        Frag
    };

    struct ShaderModuleDescription
    {
        [[nodiscard]] auto size() const noexcept -> std::size_t { return buffer.size(); }

        [[nodiscard]] auto data() const noexcept -> const char8_t* { return buffer.data(); }

        std::vector<char8_t> buffer {};
    };
} // namespace rhi

#endif // RHI_SHADER_H
