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
        std::cout << "Device: " << "{ name: " << physical_device.name
                  << ", VRAM: " << physical_device.video_ram_bytes << " }" << std::endl;
    }

    xgpu::expected<xgpu::vulkan_adapter, xgpu::Error> adapter = instance.create_adapter();

    if ( !adapter ) {
        std::cout << "Failed to create adapter: " << adapter.error().to_string() << std::endl;
    }

    instance.destroy();
}

#endif // XGPU_COMPILE_VULKAN
