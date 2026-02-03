#include "rhi/renderpass.h"
#include "expected.h"
#include "platform.h"

namespace rhi
{
    auto getAttachmentVulkanLoadOp(LoadOperation op) -> VkAttachmentLoadOp;
    auto getVulkanAttachmentStoreOp(StoreOperation op) -> VkAttachmentStoreOp;

    auto Renderpass::create(
        Device &device, 
        std::initializer_list<OpenAttachmentDescription> attachments,
        std::initializer_list<OpenSubpassDescription> subpasses) noexcept -> expected<Renderpass, Error>
    {
        switch (device.backend())
        {
            case Backend::Vulkan:
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
