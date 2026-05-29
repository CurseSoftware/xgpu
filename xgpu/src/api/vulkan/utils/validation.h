#pragma once
#include "xgpu/native/native.h"

#include <span>
#include <vector>

namespace xgpu::vk
{
    struct RequestedValidationLayer
    {
        bool             required{ true };
        std::string_view name;
    };

    class ValidationLayerManager
    {
      public:
        [[nodiscard]] explicit ValidationLayerManager() noexcept;

        /// @brief Determine if a layer is available or not
        [[nodiscard]] bool is_available(std::string_view layer_name) const noexcept;

        /// @brief Get a list of all the available layers
        [[nodiscard]] std::span<VkLayerProperties>
        layers() noexcept
        {
            return m_layers;
        }

        [[nodiscard]] expected<std::vector<const char *>, Error>
        get_requested(std::span<RequestedValidationLayer> requested_layers) const noexcept;

      private:
        std::vector<VkLayerProperties> m_layers;
    };
} // namespace xgpu::vk