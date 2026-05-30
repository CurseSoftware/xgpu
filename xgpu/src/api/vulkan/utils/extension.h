#pragma once
#include "xgpu/native/native.h"

#include <__ostream/basic_ostream.h>
#include <iostream>
#include <ranges>
#include <span>
#include <vector>

namespace xgpu::vk
{
    /// @brief The kinds of extensions that are available
    enum class ExtensionKind
    {
        Instance,
        Device
    };

    struct RequestedExtension
    {
        bool             required{ false };
        std::string_view name;
    };

    template <ExtensionKind Kind>
    class ExtensionHandler
    {
      public:
        using ExtensionList = std::vector<VkExtensionProperties>;

        [[nodiscard]] explicit ExtensionHandler() noexcept
            requires(Kind == ExtensionKind::Instance)
        {
            fetch_all();
        }

        [[nodiscard]] explicit ExtensionHandler(VkPhysicalDevice physical_device) noexcept
            requires(Kind == ExtensionKind::Device)
        {
            fetch_all(physical_device);
        }

        /// @brief Determine if extension is available
        [[nodiscard]] bool
        is_available(std::string_view extension_name) const noexcept
        {
            const auto found
                = std::ranges::find(m_extensions, extension_name, &VkExtensionProperties::extensionName);
            return found != m_extensions.end();
        }

        [[nodiscard]] expected<std::vector<const char *>, Error>
        get_requested(std::span<RequestedExtension> requested_extensions) noexcept
        {
            auto missing = std::ranges::find_if(
                requested_extensions, [&](const RequestedExtension &requested_extension) {
                    return requested_extension.required && !is_available(requested_extension.name);
                });

            if ( missing != requested_extensions.end() ) {
                return unexpected(Error::make_error(ErrorCode::ExtensionNotFound, missing->name));
            }

            auto found = requested_extensions | std::views::filter([&](const RequestedExtension &extension) {
                             return is_available(extension.name);
                         })
                         | std::views::transform(
                             [](const RequestedExtension &extension) { return extension.name.data(); });

            return std::vector<const char *>(found.begin(), found.end());
        }

      private:
        /// @brief Fetch all the available extensions
        void fetch_all() noexcept
            requires(Kind == ExtensionKind::Instance);

        /// @brief Fetch all extensions for a given device
        void fetch_all(const VkPhysicalDevice physical_device) noexcept
            requires(Kind == ExtensionKind::Device);

      private:
        ExtensionList m_extensions;
    };
} // namespace xgpu::vk