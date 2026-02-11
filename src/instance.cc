#include "xgpu/instance.h"
#include "xgpu/expected.h"
#include "xgpu/instance_context.h"
#include "xgpu/platform.h"
#include "xgpu/vk/instance.h"
#include "xgpu/vk/instance_context.h"
#include <variant>

namespace xgpu
{
    auto Instance::create(const InstanceContext& ctx) noexcept -> expected<Instance, Error>
    {
        Instance instance;
        if (std::holds_alternative<DefaultInstanceContext>(ctx))
        {
            return unexpected(Error("Creating instance from default context is not supported yet."));
        }

        // Create the vulkan instance if that is the context we are given
        if constexpr (backendIsAvailable(Backend::Vulkan))
        {
            if (std::holds_alternative<vk::InstanceContext>(ctx))
            {
                auto expected_instance = vk::Instance::create(std::get<vk::InstanceContext>(ctx));
                if (!expected_instance.has_value())
                {
                    return unexpected(expected_instance.unwrap_error());
                }

                instance = expected_instance.unwrap();

                return ok(instance);
            }
        }

        // Create the Dx12 instance if that is the context we are given
        else if constexpr (backendIsAvailable(Backend::Dx12))
        {
            // TODO: support Dx12 instances
            return unexpected(Error("DirectX 12 is not yet supported."));
        }

        // Metal handles instances differently, so we shold probably
        // do a warning here but not fail
        else if constexpr (backendIsAvailable(Backend::Metal))
        {
            return unexpected(Error("Metal is not yet supported."));
        }

        return unexpected(Error("Provided context is not supported."));
    }
} // namespace xgpu
