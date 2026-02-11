#ifndef RHI_SHADER_H
#define RHI_SHADER_H

#include "expected.h"
#include "xgpu/device.h"

#include <memory>
#include <span>
#include <vector>

namespace xgpu
{
    enum class ShaderStageFlags
    {
        Vert,
        Frag
    };

    class IShaderModule
    {
        public:
            virtual auto destroy() noexcept -> void = 0;

            virtual ~IShaderModule() = default;
    };

    class ShaderModule : public IShaderModule
    {
        // Factory
        public:
            static auto create(xgpu::Device& device, std::span<char8_t> data) noexcept -> expected<ShaderModule, Error>;

        // API
        public:
            auto destroy() noexcept -> void override { _handle->destroy(); }

            [[nodiscard]] auto handle() const noexcept -> IShaderModule* { return _handle.get(); }

        private:
            [[nodiscard]] explicit ShaderModule() = default;

        private:
            std::unique_ptr<IShaderModule> _handle { nullptr };
    };
} // namespace xgpu

#endif // RHI_SHADER_H
