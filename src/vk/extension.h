#ifndef RHI_VK_PRIV_EXTENSION_H
#define RHI_VK_PRIV_EXTENSION_H

#include "rhi/expected.h"
#include "rhi/core/log.h"
#include <cstdint>
#include <span>
#include <string_view>
#include <type_traits>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace xgpu::vk::priv
{
    enum class ExtensionSource
    {
        Instance,
        PhysicalDevice
    };

    struct RequestedExtension
    {
        bool required { true };
        std::string_view extension_name;
    };

    class ExtensionHandler
    {
        public:
            [[nodiscard]] explicit ExtensionHandler() noexcept = default;

            [[nodiscard]] auto isSupported(std::string_view extension_name) const noexcept -> bool
            {
                for (const auto& extension : _extensions)
                {
                    if (extension.extensionName == extension_name)
                    {
                        return true;
                    }
                }

                return false;
            }
    
            template <ExtensionSource T> 
            auto fetchAll() noexcept -> void {}

            [[nodiscard]] auto getRequested(std::span<RequestedExtension> requested_extensions) -> expected<std::vector<const char*>, std::string_view>
            {
                std::vector<const char*> extensions_found {};

                for (auto requested_extension : requested_extensions)
                {
                    if (!isSupported(requested_extension.extension_name))
                    {
                        if (requested_extension.required)
                        {
                            return unexpected(requested_extension.extension_name);
                        }
                    }
                    else
                    {
                        extensions_found.push_back(requested_extension.extension_name.data());
                    }
                }

                return ok(extensions_found);
            }
        
            // Private methods
        private:
            auto fetchAll() noexcept -> void;

        // Private fields
        private:
            std::vector<VkExtensionProperties> _extensions {};
    };
    
    template <>
    auto ExtensionHandler::fetchAll<ExtensionSource::Instance>() noexcept -> void
    {
        constexpr const char* dummy_name { nullptr };
        constexpr VkExtensionProperties* dummy_properties { nullptr };
        std::uint32_t extension_count { 0 };

        vkEnumerateInstanceExtensionProperties(dummy_name, &extension_count, dummy_properties);
        _extensions.resize(extension_count);
        vkEnumerateInstanceExtensionProperties(dummy_name, &extension_count, _extensions.data());
    }
    
    // NOLINTNEXTLINE
    template <>
    auto ExtensionHandler::fetchAll<ExtensionSource::PhysicalDevice>() noexcept -> void
    {
        // TODO:
    }
} // namespace xgpu::vk::priv

#endif // RHI_VK_PRIV_EXTENSION_H
