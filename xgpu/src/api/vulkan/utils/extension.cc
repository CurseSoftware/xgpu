#include "extension.h"

namespace xgpu::vk
{
    // template <>
    // ExtensionHandler<ExtensionKind::Instance>::ExtensionHandler() noexcept
    // {
    //     fetch_all();
    // }
    //
    // template <>
    // ExtensionHandler<ExtensionKind::Device>::ExtensionHandler(const VkPhysicalDevice physical_device)
    // noexcept
    // {
    //     fetch_all(physical_device);
    // }

    template <>
    void
    ExtensionHandler<ExtensionKind::Instance>::fetch_all() noexcept
    {
        constexpr const char            *dummy_extension_name{ nullptr };
        constexpr VkExtensionProperties *dummy_extension_properties{ nullptr };
        std::uint32_t                    extension_count{ 0 };

        vkEnumerateInstanceExtensionProperties(
            dummy_extension_name, std::addressof(extension_count), dummy_extension_properties);
        m_extensions.resize(extension_count);
        vkEnumerateInstanceExtensionProperties(
            dummy_extension_name, std::addressof(extension_count), m_extensions.data());
    }

    template <>
    void
    ExtensionHandler<ExtensionKind::Device>::fetch_all(const VkPhysicalDevice physical_device) noexcept
    {
        constexpr const char            *layer_name{ nullptr };
        constexpr VkExtensionProperties *dummy_extension_properties{ nullptr };
        std::uint32_t                    extension_count{ 0 };

        (void)vkEnumerateDeviceExtensionProperties(
            physical_device, layer_name, std::addressof(extension_count), dummy_extension_properties);
        m_extensions.resize(extension_count);
        (void)vkEnumerateDeviceExtensionProperties(
            physical_device, layer_name, std::addressof(extension_count), m_extensions.data());
    }
} // namespace xgpu::vk