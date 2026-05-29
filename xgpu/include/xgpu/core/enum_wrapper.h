#pragma once
#include "concepts.h"

#include <string>

namespace xgpu::utils
{
    /// @brief A convenience wrapper around an enum for commonly useful utilities
    template <concepts::Enum E>
    class EnumWrapper
    {
      public:
        using Enum = E;

        /// @brief Default construction of EnumWrapper
        [[nodiscard]] constexpr explicit EnumWrapper() noexcept = default;

        /// @brief Initialize EnumWrapper from value
        [[nodiscard]] constexpr explicit EnumWrapper(E e) noexcept : m_value{ e } {}

        /// @brief Get the raw enum value
        [[nodiscard]] constexpr Enum
        value() const noexcept
        {
            return m_value;
        }

        /// @brief Get the raw enum value
        [[nodiscard]] constexpr Enum
        raw() const noexcept
        {
            return m_value;
        }

        [[nodiscard]] constexpr std::string to_string() const noexcept;

        constexpr explicit(false) operator std::string() const noexcept { return to_string(); }

        constexpr explicit(false) operator E() const noexcept { return m_value; }

      private:
        Enum m_value;
    };
} // namespace xgpu::utils