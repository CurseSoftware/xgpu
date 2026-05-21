#pragma once
#include "xgpu/core/core.h"

namespace xgpu::components::detail
{
    template <typename Context>
    struct ContextBase
    {
        using context_type = Context;

        [[nodiscard]] explicit ContextBase(Context ctx) noexcept : m_context{ std::move(ctx) } {}

        /// @brief Get a reference to the held context
        [[nodiscard]] context_type &
        context() noexcept
        {
            return m_context;
        }

        /// @brief Get a const reference to the held context
        [[nodiscard]] const context_type &
        context() const noexcept
        {
            return m_context;
        }

      private:
        context_type m_context;
    };

    template <typename Derived, typename Context>
    struct ContextAccessor
    {
        [[nodiscard]] Context &
        context() noexcept
        {
            return derived()->template ContextBase<Context>::context();
        }

        [[nodiscard]] const Context &
        context() const noexcept
        {
            return derived()->template ContextBase<Context>::context();
        }

      protected:
        [[nodiscard]] Derived *
        derived() noexcept
        {
            return static_cast<Derived *>(this);
        }

        [[nodiscard]] const Derived *
        derived() const noexcept
        {
            return static_cast<Derived *>(this);
        }
    };
} // namespace xgpu::traits::detail
