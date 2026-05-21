#pragma once
#include "concepts.h"
#include "enum_wrapper.h"
#include <cstdint>

namespace xgpu
{
    template <concepts::Enum E, concepts::Integral N = std::uint32_t>
    class Flag
    {
      public:
        using FlagType    = E;
        using NumericType = N;

        /// @brief Initialize the flag to value of 0
        [[nodiscard]] constexpr Flag() noexcept : m_value{ 0 } {}

        /// @brief Initialize a flag from a value
        [[nodiscard]] explicit Flag(FlagType flag) noexcept : m_value{ static_cast<N>(flag) } {}

        /// @brief Get the numeric value held by the FLag
        [[nodiscard]] NumericType
        value() const noexcept
        {
            return m_value;
        }

        /// @brief Get the numeric value held by the FLag
        [[nodiscard]] NumericType
        as_numeric() const noexcept
        {
            return m_value;
        }

        /// @brief Convertible to N
        constexpr explicit
        operator NumericType() const noexcept
        {
            return as_numeric();
        }

        /// @brief Use the * operator to get the numeric value
        constexpr NumericType
        operator*() const noexcept
        {
            return as_numeric();
        }

        /// @brief Comparison to another Flag
        constexpr bool
        operator==(const Flag &rhs) const noexcept
        {
            return m_value == rhs.m_value;
        }

        /// @brief | the flag's current value with the value of the rhs
        constexpr Flag &
        operator|=(const Flag &rhs) noexcept
        {
            m_value |= rhs.m_value;
            return *this;
        }

        /// @brief Support |= operator with raw Enum type
        constexpr friend Flag &
        operator|=(Flag &lhs, FlagType rhs) noexcept
        {
            lhs.m_value |= static_cast<N>(rhs);
            return lhs;
        }

        /// @brief Support |= operator with raw Enum type
        constexpr friend Flag
        operator|(const Flag &lhs, FlagType rhs) noexcept
        {
            return Flag{ lhs | rhs };
        }

        /// @brief Create a new flag from this value | the rhs value
        [[nodiscard]] constexpr Flag
        operator|(const Flag &rhs) const noexcept
        {
            return Flag{ m_value | rhs.m_value };
        }

        /// @brief Determine if the current Flag contains a specific enum value
        [[nodiscard]] constexpr bool
        contains(FlagType flag) const noexcept
        {
            return value() & static_cast<N>(flag);
        }

      private:
        NumericType m_value{ 0 };
    };
} // namespace xgpu