#include "expected.h"
#include "vk/instance_context.h"
#include "vk/instance.h"

namespace rhi::vk
{
    auto rhi::vk::Instance::create(const rhi::vk::InstanceContext& ctx) noexcept -> expected<rhi::vk::Instance, Error>
    {
        Instance inst = Instance();

        return ok(inst);
    }
} // namespace rhi::vk
