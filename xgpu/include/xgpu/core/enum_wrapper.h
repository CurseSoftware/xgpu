#pragma once
#include "xgpu/core/core.h"

#include <string>

namespace xgpu::utils
{
    template <concepts::Enum E>
    class EnumWrapper
    {
      public:
        using Enum = E;

        [[nodiscard]] constexpr explicit EnumWrapper(E e) noexcept : m_value{ e } {}

        [[nodiscard]] Enum
        value() const noexcept
        {
            return m_value;
        }

        [[nodiscard]] std::string to_string() const noexcept;

        constexpr explicit(false) operator std::string() const noexcept { return to_string(); }

        constexpr explicit(false) operator E() const noexcept { return m_value; }

      private:
        Enum m_value;
    };
} // namespace xgpu::utils