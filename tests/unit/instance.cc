#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <xgpu/xgpu.h>

TEST_CASE("instance", "[instance]")
{
    xgpu::Instance instance = xgpu::create_instance(xgpu::InstanceDesc{ .name = "test" }).value();
    const std::span<xgpu::data::PhysicalDevice> available_devices = instance.enumerate_devices();
    REQUIRE_FALSE(available_devices.empty());
}

TEST_CASE("default adapter", "[instance]")
{
    xgpu::Instance instance = xgpu::create_instance(xgpu::InstanceDesc{ .name = "test" }).value();
    xgpu::Adapter  adapter  = instance.create_adapter().value();

    REQUIRE(adapter.physical_device() == instance.default_physical_device());
}

TEST_CASE("adapter from specified physical device", "[instance]")
{
    xgpu::Instance             instance = xgpu::create_instance(xgpu::InstanceDesc{ .name = "test" }).value();
    xgpu::data::PhysicalDevice physical_device = instance.enumerate_devices().front();
    xgpu::Adapter              adapter         = instance.create_adapter(physical_device).value();

    REQUIRE(adapter.physical_device() == physical_device);
}
