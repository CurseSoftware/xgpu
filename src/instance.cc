#include "rhi/instance.h"
#include "expected.h"
#include "instance_context.h"
#include "platform.h"
#include "vk/instance.h"
#include "vk/instance_context.h"
#include <variant>

namespace rhi
{
    auto Instance::create(const InstanceContext& ctx) noexcept -> expected<Instance, Error>
    {
        if (std::holds_alternative<DefaultInstanceContext>(ctx))
        {
            return unexpected(Error {});
        }

        // Create the vulkan instance if that is the context we are given
        if constexpr (backendIsAvailable(Backend::Vulkan))
        {
            if (std::holds_alternative<vk::InstanceContext>(ctx))
            {
                return vk::Instance::create(std::get<vk::InstanceContext>(ctx));
            }
        }

        // Create the Dx12 instance if that is the context we are given
        else if constexpr (backendIsAvailable(Backend::Dx12))
        {
            // TODO: support Dx12 instances
        }

        // Metal handles instances differently, so we shold probably
        // do a warning here but not fail
        else if constexpr (backendIsAvailable(Backend::Metal))
        {
            // TODO: support Dx12 instances
        }

        return unexpected(Error {});
    }
} // namespace rhi
