#include <xgpu/xgpu.h>
#include <catch2/catch_test_macros.hpp>

TEST_CASE( "test", "[test]" ) {
  REQUIRE(xgpu::test());
}