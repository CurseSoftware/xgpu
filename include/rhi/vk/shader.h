#ifndef RHI_VK_SHADER_H
#define RHI_VK_SHADER_H

#include "rhi/vk/core.h"
#include "rhi/device.h"
#include "rhi/expected.h"
#include "rhi/shader.h"

#include <span>

namespace xgpu::vk
{
    class ShaderModule : public IShaderModule
    {
        // Factory
        public:
            static auto from_open(xgpu::Device& device, std::span<char8_t> data) -> expected<vk::ShaderModule, Error>;

        // API
        public:
            auto destroy() noexcept -> void override;

            auto data() const noexcept -> const char8_t* { return _code.data(); }

            auto code() const noexcept -> const std::uint32_t* { return reinterpret_cast<const std::uint32_t*>(_code.data()); }

            auto handle() const noexcept -> VkShaderModule { return _handle; }

        // Private special member functions
        private:
            [[nodiscard]] explicit ShaderModule() noexcept = default;
            [[nodiscard]] explicit ShaderModule(VkDevice device) noexcept 
                : _device{ device }
            {}
            [[nodiscard]] explicit ShaderModule(VkDevice device, std::span<char8_t> code) noexcept 
                : _device{ device }
                , _code{ code }
            {}

        // Private member fields
        private:
            VkDevice _device         { VK_NULL_HANDLE };
            VkShaderModule _handle   { VK_NULL_HANDLE };
            std::span<char8_t> _code {};
    };
} // namespace xgpu::vk

#endif // RHI_VK_SHADER_H
