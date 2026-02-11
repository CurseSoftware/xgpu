#ifndef RHI_DESCRIPTOR_SET_H
#define RHI_DESCRIPTOR_SET_H

#include "rhi/expected.h"
#include "rhi/error.h"
#include "rhi/shader.h"
#include "rhi/types.h"
#include "rhi/device.h"
#include <memory>
#include <span>

namespace xgpu
{
    struct DescriptorSetLayoutBinding
    {
        std::uint32_t binding          { 0 };
        DescriptorType descriptor_type { DescriptorType::UniformBuffer };
        ShaderStageFlags shader_stage  { ShaderStageFlags::Vert };
        std::uint32_t descriptor_count { 1 };
    };

    struct DescriptorSetLayoutDescription
    {
        std::span<const DescriptorSetLayoutBinding> bindings {};
    };

    class IDescriptorSetLayout
    {
        public:
            virtual auto destroy() noexcept -> void = 0;
    };

    class DescriptorSetLayout : public IDescriptorSetLayout
    {
        // Factory
        public:
            static auto create(Device& device, const DescriptorSetLayoutDescription& description) noexcept -> expected<DescriptorSetLayout, Error>;

        // API
        public:
            auto destroy() noexcept -> void override { _handle->destroy(); }

            [[nodiscard]] auto handle() const noexcept -> IDescriptorSetLayout* { return _handle.get(); }

        // Private special members
        private:
            [[nodiscard]] explicit DescriptorSetLayout() noexcept = default;

        // Private fields
        private:
            std::unique_ptr<IDescriptorSetLayout> _handle { nullptr };
    };
} // namespace xgpu

#endif // RHI_DESCRIPTOR_SET_H
