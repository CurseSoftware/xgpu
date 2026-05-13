#ifndef RHI_VK_VALIDATION_H
#define RHI_VK_VALIDATION_H

#include "xgpu/expected.h"
#include <span>
#include <vulkan/vulkan_core.h>

#include <string_view>
#include <vector>

namespace xgpu::vk::priv
{
    struct RequestedLayer
    {
        // Whether the layer is required or not
        bool required { true };

        // The name of the requested layer
        std::string_view layer_name;
    };

    class ValidationLayerManager
    {
        public:
            [[nodiscard]] explicit ValidationLayerManager() noexcept;

            // Return `true` if the layer name is supported
            [[nodiscard]] auto isSupported(std::string_view layer_name) const noexcept -> bool;

            [[nodiscard]] auto getRequestedLayers(std::span<RequestedLayer> layers) noexcept -> expected<std::vector<const char*>, std::string_view>;

            [[nodiscard]] auto layers() const noexcept -> const std::vector<VkLayerProperties> { return _layers; }
        
        private:
            std::vector<VkLayerProperties> _layers {};
    };
} // namespace xgpu::vk::priv

#endif // RHI_VK_VALIDATION_H
