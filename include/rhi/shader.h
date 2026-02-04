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

    class ShaderModule
    {
        // Factory
        public:
            [[nodiscard]] static auto create(rhi::Device& p_device, const std::vector<char8_t>& data) noexcept -> ShaderModule;

        // API
        public:
            [[nodiscard]] auto size() const noexcept -> std::size_t { return _data.size(); }

            [[nodiscard]] auto data() const noexcept -> const char8_t* { return _data.data(); }

        private:
            [[nodiscard]] explicit ShaderModule(rhi::Device& p_device, const std::vector<char8_t>& p_data)
                : _device{ p_device }
                , _data{ p_data }
            {}

        private:
            rhi::Device& _device;
            std::vector<char8_t> _data {};
    };
} // namespace rhi

#endif // RHI_SHADER_H
