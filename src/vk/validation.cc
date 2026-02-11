#include "validation.h"
#include "expected.h"
#include <cstdint>
#include <vulkan/vulkan_core.h>

namespace xgpu::vk::priv
{
    ValidationLayerManager::ValidationLayerManager() noexcept
    {
        std::uint32_t layer_count { 0 };
        VkLayerProperties* dummy_properties { nullptr };
        
        vkEnumerateInstanceLayerProperties(&layer_count, dummy_properties);
        _layers.resize(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, _layers.data());
    }

    auto ValidationLayerManager::isSupported(std::string_view layer_name) const noexcept -> bool
    {
        for (const auto& layer : _layers)
        {
            if (layer.layerName == layer_name)
            {
                return true;
            }
        }

        return false;
    }

    auto ValidationLayerManager::getRequestedLayers(std::span<RequestedLayer> requested_layers) noexcept -> expected<std::vector<const char*>, std::string_view>
    {
        std::vector<const char*> layers_found {};

        for (const auto& requested_layer : requested_layers)
        {
            if (!isSupported(requested_layer.layer_name))
            {
                if (requested_layer.required)
                {
                    return unexpected(requested_layer.layer_name);
                }
            }
            else
            {
                layers_found.push_back(requested_layer.layer_name.data());
            }
        }

        return ok(layers_found);
    }
} // namespace xgpu::vk::priv
