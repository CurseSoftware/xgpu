#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <xgpu/xgpu.h>

TEST_CASE("instance", "[instance]")
{
    xgpu::Instance instance{ xgpu::InstanceDesc{ .name = "test" } };
    xgpu::Adapter  adapter = instance.create_adapter();

    std::cout << "test\n";
}