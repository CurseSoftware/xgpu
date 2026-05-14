#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <xgpu/xgpu.h>

TEST_CASE("error", "[error]")
{
    std::string            message = "my message";
    const xgpu::ErrorCode  code    = xgpu::ErrorCode::InstanceError;
    const xgpu::ErrorLevel level   = xgpu::ErrorLevel::Error;
    const xgpu::Error      error{ level, code, message };

    REQUIRE(message == std::string(error.message()));
    REQUIRE(error.code() == code);
    REQUIRE(error.level() == level);
}

TEST_CASE("error factory", "[error]")
{
    std::string           message = "my message";
    const xgpu::ErrorCode code    = xgpu::ErrorCode::InstanceError;

    const xgpu::Error     warning = xgpu::Error::make_warning(code, message);
    const xgpu::Error     error   = xgpu::Error::make_error(code, message);
    const xgpu::Error     fatal   = xgpu::Error::make_fatal(code, message);

    REQUIRE(message == std::string(error.message()));
    REQUIRE(warning.code() == code);
    REQUIRE(warning.level() == xgpu::ErrorLevel::Warning);

    REQUIRE(error.code() == code);
    REQUIRE(error.level() == xgpu::ErrorLevel::Error);

    REQUIRE(fatal.code() == code);
    REQUIRE(fatal.level() == xgpu::ErrorLevel::Fatal);

    std::cout << warning.to_string() << std::endl;
    std::cout << error.to_string() << std::endl;
    std::cout << fatal.to_string() << std::endl;
}
