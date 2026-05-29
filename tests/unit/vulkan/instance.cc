#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <xgpu/xgpu.h>

#ifdef XGPU_COMPILE_VULKAN

TEST_CASE("vulkan instance", "[vulkan]")
{
    xgpu::expected<xgpu::vulkan_instance, xgpu::Error> e_instance
        = xgpu::create_instance<xgpu::GraphicsApi::Vulkan>({ .name = "test" });

    if ( !e_instance ) {
        std::cerr << e_instance.error().to_string() << std::endl;
    }
}

#endif // XGPU_COMPILE_VULKAN
