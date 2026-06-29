#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <xgpu/xgpu.h>

#ifdef XGPU_COMPILE_VULKAN

TEST_CASE("vulkan instance", "[vulkan]")
{
    xgpu::expected<xgpu::vulkan_instance, xgpu::Error> exp_instance
        = xgpu::create_instance<xgpu::GraphicsApi::Vulkan>({ .name = "test" });

    if ( !exp_instance ) {
        std::cerr << exp_instance.error().to_string() << std::endl;
    }

    xgpu::vulkan_instance                 instance = exp_instance.value();

    std::span<xgpu::data::PhysicalDevice> physical_devices = instance.enumerate_devices();
    for ( const xgpu::data::PhysicalDevice &physical_device : physical_devices ) {
        std::cout << "Physical Device: {\n";
        std::cout << "\tName: " << physical_device.name << "\n";
        std::cout << "\tVRAM: " << physical_device.video_ram_bytes << "\n";
        std::cout << "\tCapabilities: {\n";
        std::cout << "\t\tCompute:  " << std::boolalpha << physical_device.capabilities.compute << "\n";
        std::cout << "\t\tGraphics: " << std::boolalpha << physical_device.capabilities.graphics << "\n";
        std::cout << "\t}\n";
        std::cout << "}\n";
    }

    xgpu::expected<xgpu::vulkan_adapter, xgpu::Error> adapter = instance.create_adapter();

    if ( !adapter ) {
        std::cout << "Failed to create adapter: " << adapter.error().to_string() << std::endl;
    }

    adapter->destroy();
    instance.destroy();
}

#endif // XGPU_COMPILE_VULKAN
