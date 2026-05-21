#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <xgpu/xgpu.h>

TEST_CASE("flag", "[flag]")
{
    enum Test : std::uint32_t
    {
        One   = 0x00000001,
        Two   = 0x00000002,
        Three = 0x00000004,
    };

    xgpu::Flag<Test> flag{ Test::One };
    REQUIRE(flag.contains(Test::One));
    REQUIRE_FALSE(flag.contains(Test::Two));
    REQUIRE_FALSE(flag.contains(Test::Three));

    flag |= Test::Two;

    REQUIRE(flag.contains(Test::One));
    REQUIRE(flag.contains(Test::Two));
    REQUIRE_FALSE(flag.contains(Test::Three));

    flag |= Test::Three;

    REQUIRE(flag.contains(Test::One));
    REQUIRE(flag.contains(Test::Two));
    REQUIRE(flag.contains(Test::Three));
}
