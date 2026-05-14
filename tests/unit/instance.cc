#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <xgpu/xgpu.h>

TEST_CASE("instance", "[instance]")
{
    xgpu::Instance                        instance{ xgpu::InstanceDesc{ .name = "test" } };

    std::span<xgpu::data::PhysicalDevice> available_devices = instance.enumerate_devices();

    for ( const xgpu::data::PhysicalDevice &device : available_devices ) {
        std::cout << "Device:" << std::endl;
        std::cout << "\tName: " << device.name << std::endl;
        std::cout << "\tVRAM: " << device.video_ram_bytes << " bytes" << std::endl;
    }

    xgpu::data::PhysicalDevice default_device = instance.default_physical_device();
    std::cout << "Default device:" << std::endl;
    std::cout << "\tName: " << default_device.name << std::endl;
    std::cout << "\tVRAM: " << default_device.video_ram_bytes << " bytes" << std::endl;

    xgpu::Adapter adapter = instance.create_adapter(default_device);
}