#ifndef RHI_CORE_FLAGS_H
#define RHI_CORE_FLAGS_H

#include <concepts>
#include <type_traits>
namespace xgpu
{
    template <typename T>
    concept ValidFlagType = std::is_integral_v<T>;

    // This is a utility for creating an interface where rhi flags can be used
    // agnostic of the underlying graphics API
    template <typename Derived, ValidFlagType T>
    class Flags
    {
        // Info
        public:
            using ValueType = T;

        // Special members
        public:
            // Default constructor initializes the initial value to 0
            constexpr Flags() noexcept = default;

            constexpr Flags(ValueType value)
                : _value{ value }
            {}

            constexpr operator ValueType() const { return _value; }
        
        // Operator overloads
        public:
            constexpr auto operator|(Flags rhs) const -> Derived
            {
                return Derived(_value | rhs._value);
            }

            constexpr auto operator&(Flags rhs) const -> Derived& 
            {
                return Derived(_value & rhs._value);
            }

            constexpr auto operator|=(Flags rhs) -> Derived&
            {
                _value |= rhs._value;
                return *this;
            }

            constexpr auto operator&=(Flags rhs) -> Derived&
            {
                _value &= rhs._value;
                return *this;
            }

            constexpr auto operator=(Flags rhs) -> bool
            {
                return _value = rhs._value;
            }

        // Getter
        public:
            [[nodiscard]] constexpr auto get() -> ValueType { return _value; }

        // Private fields
        private:
            T _value { 0 };
    };
} // namespace xgpu

#endif // RHI_CORE_FLAGS_H
