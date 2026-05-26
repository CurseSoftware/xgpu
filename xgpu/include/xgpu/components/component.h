#pragma once
#include <concepts>

namespace xgpu::components
{
    namespace ctx
    {
        struct DefaultContext
        {
        };
    } // namespace ctx

    /// @brief Unspecialized version of a component.
    /// @note This should remain undefined, the only definitions should be specializations
    template <typename Context>
    struct Component : Context
    {
        using context_t = Context;

        Context &
        context() noexcept
        {
            return *static_cast<Context *>(this);
        }

        const Context &
        context() const noexcept
        {
            return *static_cast<Context *>(this);
        }
    };

    /// @brief Type trait for whether a type is a component
    template <typename T>
    struct is_component : std::false_type
    {
    };

    template <typename Context>
    struct is_component<Component<Context>> : std::true_type
    {
    };

    template <typename T>
    static constexpr bool is_component_v = is_component<T>::value;

    /// @brief Interface and type requirements for a valid Component
    /// @note This is used to ensure that specializations have valid minimum requirements
    template <typename C>
    concept IComponent = requires(C component, const C const_component) {
        // Ensure that a component has a valid associated context type
        typename C::context_t;

        requires requires {
            { component.context() } -> std::same_as<const typename C::context_t &>;
        } || requires {
            { const_component.context() } -> std::same_as<typename C::context_t &>;
        };
    } && is_component_v<C> && std::derived_from<C, typename C::context_t>;
} // namespace xgpu::components