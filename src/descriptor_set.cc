#include "rhi/descriptor_set.h"
#include "rhi/vk/descriptor_set.h"
#include "platform.h"
#include <memory>

namespace rhi
{
    auto DescriptorSetLayout::create(Device& device, const DescriptorSetLayoutDescription& description) noexcept -> expected<DescriptorSetLayout, Error>
    {
        DescriptorSetLayout descriptor_set {};

        switch (device.backend())
        {
            case Backend::Vulkan:
            {
#ifdef RHI_COMPILE_VULKAN_BACKEND
                auto expected_descriptor_set = vk::DescriptorSetLayout::from_open(device, description);
                if (!expected_descriptor_set.has_value())
                {
                    return unexpected( expected_descriptor_set.unwrap_error() );
                }
                descriptor_set._handle = std::make_unique<vk::DescriptorSetLayout>(expected_descriptor_set.unwrap());
#else
                return unexpected( Error("Cannot create vulkan descriptor set when vulkan backend is not compiled") );
#endif // RHI_COMPILE_VULKAN_BACKEND
            } break;
            case Backend::Dx12:
                return unexpected( Error("DirectX12 backend not supported") );
                break;
            case Backend::Metal:
                return unexpected( Error("Metal backend not supported") );
                break;
        }

        return ok(descriptor_set);
    }
} // namespace rhi
