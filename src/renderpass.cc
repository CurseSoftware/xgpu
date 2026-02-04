#include "rhi/renderpass.h"
#include "expected.h"
#include "platform.h"
#include "vk/renderpass.h"
#include <memory>

namespace rhi
{
    auto Renderpass::create(
        Device &p_device,
        std::initializer_list<OpenAttachmentDescription> p_attachments,
        std::initializer_list<OpenSubpassDescription> p_subpasses) noexcept -> expected<Renderpass, Error>
    {
        Renderpass renderpass {};
        std::vector<OpenAttachmentDescription> attachments { p_attachments };
        std::vector<OpenSubpassDescription> subpasses { p_subpasses };
        
        switch (p_device.backend())
        {
            case Backend::Vulkan:
#ifdef RHI_COMPILE_VULKAN_BACKEND
            {
                auto expected_renderpass = vk::Renderpass::from_open(
                    p_device, 
                    attachments, 
                    subpasses
                );
                if (!expected_renderpass.has_value())
                {
                    return unexpected(expected_renderpass.unwrap_error());
                }

                renderpass._handle = std::make_unique<vk::Renderpass>(expected_renderpass.unwrap());
                return ok(renderpass);
            } break;
#else 
            return unexpected( Error("Cannot initialize a vulkan renderpass when vulkan backend is not compiled") );
#endif // RHI_COMPILE_VULKAN_BACKEND
            case Backend::Dx12:
                return unexpected( Error("Renderpass not supported on DirectX backend yet.") );
            case Backend::Metal:
                return unexpected( Error("Renderpass not supported on Metal backend yet.") );
            default:
                return unexpected( Error("Unsupported renderer backend.") );
        }
    
        return unexpected( Error("Unabled to create renderpass") );
    }
} // namespace rhi
