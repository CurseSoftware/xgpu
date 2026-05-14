#pragma once
#include <type_traits>

namespace xgpu::concepts
{
    /// @brief Concept for any valid integral type
    template <typename T>
    concept Integral = std::is_integral_v<T>;

    /// @brief Concept for any valid floating point type
    template <typename T>
    concept FloatingPoint = std::is_floating_point_v<T>;

    /// @brief Concept for any numeric type (floating || integer)
    template <typename T>
    concept Numeric = Integral<T> || FloatingPoint<T>;

    /// @brief Concept for any enum type
    template <typename T>
    concept Enum = std::is_enum_v<T>;
} // namespace xgpu::concepts