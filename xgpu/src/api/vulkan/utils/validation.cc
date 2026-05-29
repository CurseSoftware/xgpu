#include "validation.h"
#include <ranges>

namespace xgpu::vk
{
    ValidationLayerManager::ValidationLayerManager() noexcept
    {
        std::uint32_t      layer_count{ 0 };
        VkLayerProperties *dummy_layers{ nullptr };

        vkEnumerateInstanceLayerProperties(std::addressof(layer_count), dummy_layers);
        m_layers.resize(layer_count);
        vkEnumerateInstanceLayerProperties(std::addressof(layer_count), m_layers.data());
    }

    bool
    ValidationLayerManager::is_available(std::string_view layer_name) const noexcept
    {
        const auto found = std::ranges::find(m_layers, layer_name, &VkLayerProperties::layerName);
        return found != m_layers.end();
    }

    expected<std::vector<const char *>, Error>
    ValidationLayerManager::get_requested(std::span<RequestedValidationLayer> requested_layers) const noexcept
    {
        auto missing
            = std::ranges::find_if(requested_layers, [&](const RequestedValidationLayer &requested_layer) {
                  return requested_layer.required && !is_available(requested_layer.name);
              });

        if ( missing != requested_layers.end() ) {
            return unexpected(
                Error::make_error(
                    ErrorCode::InstanceError, std::format("Missing validation layer: {}", missing->name)));
        }

        auto found = requested_layers
                     | std::views::filter([&](const RequestedValidationLayer &requested_layer) {
                           return is_available(requested_layer.name);
                       })
                     | std::views::transform(
                         [](const RequestedValidationLayer &layer) { return layer.name.data(); });

        return std::vector<const char *>(found.begin(), found.end());
    }
} // namespace xgpu::vk