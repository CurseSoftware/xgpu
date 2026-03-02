#ifndef RHI_EXPECTED_H
#define RHI_EXPECTED_H
#include <concepts>
#include <exception>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include <string>

namespace xgpu
{
    template <typename T, typename E>
    class expected;

    class bad_expected_access : public std::exception
    {
        public:
            bad_expected_access() : _message{ "bad xgpu::expected access" } {}
            bad_expected_access(const std::string& message) : _message{ std::string("bad xgpu::expected access") + std::move(message) } {}
            
            auto what() const noexcept -> const char* override { return _message.c_str(); }
        private:
            std::string _message;
    };

    class bad_expected_error_access : public std::exception
    {
        public:
            auto what() const noexcept -> const char* override { return "bad rhi::expected error access"; }
    };

    template <typename E>
    class unexpected
    {
    public:
        unexpected() = delete;

        constexpr explicit unexpected(const E& e) noexcept : _value{ e } {}
        constexpr explicit unexpected(E&& e) noexcept : _value{ std::move(e) } {}

        [[nodiscard]] constexpr const E& value() const & noexcept { return _value; }
        [[nodiscard]] constexpr const E&& value() const && noexcept { return std::move(_value); }

        template <typename T>
        constexpr explicit operator expected<T, E>() const & noexcept
        {
            return expected<T, E>{ unexpected(_value) };
        }

        template <typename T>
        constexpr explicit operator expected<T, E>() const && noexcept
        {
            return expected<T, E>{ unexpected(std::move(_value)) };
        }

    private:
        E _value;
    };

    template <typename T>
    class ok
    {
    public:
        constexpr ok(T& value)
        requires std::is_copy_constructible_v<T>
        : _value{ value } {}

        constexpr ok(T& value)
        requires (std::is_move_constructible_v<T> && !std::is_copy_constructible_v<T>)
        : _value{ std::move(value) } {}

        constexpr ok(const T& value)
        requires std::is_copy_constructible_v<T>
        : _value{ value } {}

        constexpr ok(const T& value)
        requires (std::is_move_constructible_v<T> && !std::is_copy_constructible_v<T>)
        : _value{ std::move(value) } {}

        constexpr ok(T&& value) : _value{ std::move(value) } {}
        constexpr ok(const T&& value) : _value{ std::move(value) } {}

        [[nodiscard]] constexpr
        auto value() const & noexcept -> T const&
        requires std::is_copy_constructible_v<T>
        {
            return _value;
        }

        [[nodiscard]] constexpr
        auto value() & noexcept -> T&
        requires std::is_copy_constructible_v<T>
        {
            return _value;
        }

        [[nodiscard]] constexpr
        auto value() const & noexcept -> T const&
        requires (std::is_move_constructible_v<T> && !std::is_copy_constructible_v<T>)
        {
            return _value;
        }

        [[nodiscard]] constexpr
        auto value() & noexcept -> T&
        requires (std::is_move_constructible_v<T> && !std::is_copy_constructible_v<T>)
        {
            return _value;
        }

        [[nodiscard]] constexpr
        auto value() const && noexcept -> T const&&
        {
            return std::move(_value);
        }

        [[nodiscard]] constexpr T&& value() && noexcept
        {
            return std::move(_value);
        }

        [[nodiscard]] constexpr ok& operator=(const ok& other) noexcept
        {
            _value = other._value;
            return *this;
        }
        [[nodiscard]] constexpr ok& operator=(ok&& other) noexcept
        {
            _value = std::move(other._value);
            return *this;
        }

        template <typename E>
        constexpr explicit operator expected<T, E>() const & noexcept
        {
            return expected<T, E>( ok(_value) );
        }

        template <typename E>
        constexpr explicit operator expected<T, E>() const && noexcept
        {
            return expected<T, E>( ok(std::move(_value)) );
        }

        ~ok() = default;

    private:
        T _value;
    };


    template <typename T>
    ok(T) -> ok<T>;

    template <typename T, typename E>
    class expected
    {
    public:
        constexpr expected() requires std::default_initializable<E> : _value{ E() } {}

        constexpr expected() requires std::default_initializable<T> : _value {T()} {}

        constexpr expected(ok<T>& value) : _value{ value.value() }, _is_ok{ true } {}

        constexpr expected(ok<T>&& value) : _value{ std::move(value.value()) }, _is_ok{ true } {}

        [[nodiscard]] expected(unexpected<E> value) noexcept : _value{ value.value() }, _is_ok { false } {}

        template <typename U = T>
        // requires std::same_as<U, T>
        [[nodiscard]] explicit expected(ok<U> value) noexcept : _value{ value.value() }
        {
            static_assert(std::is_constructible_v<T, U>(), "Cannot construct instance  of expected<T> from non-convertible type");
        }

        template <typename U>
        [[nodiscard]] explicit expected(unexpected<U> value) noexcept : _value{ value.value() }
        {
            static_assert(std::is_constructible_v<T, U>(), "Cannot construct instance  of expected<T> from non-convertible type");
        }

        constexpr expected(const expected&)
        requires (
            std::copy_constructible<T> &&
            std::copy_constructible<E> &&
            std::is_trivially_copy_constructible_v<T> &&
            std::is_trivially_copy_constructible_v<E>
        ) = default;

        constexpr expected(const expected& other)
        requires (
            std::copy_constructible<T> &&
            std::copy_constructible<E>
        ) : _is_ok{ other._is_ok }
        {
            std::construct_at(std::addressof(this->_value), other._value);
            // _is_ok = other._is_ok;
            // _value = other._value;
        }
        
        template <typename U>
        constexpr expected(const expected<U, E>& other)
        requires (
                std::is_base_of_v<T, U> &&
                std::is_default_constructible_v<E>
        ) : _value{ E{} }, _is_ok{ other._is_ok }
        {
            if (other._is_ok)
            {
                _value = std::get<U>(other._value);
            }
            // std::construct_at(std::addressof(this->_value), other._value);
            // _is_ok = other._is_ok;
            // _value = other._value;
        }

        constexpr expected& operator=(const expected& other) noexcept
        {
            if (*this != other)
            {
                _is_ok = other._is_ok;
                _value = other._value;
            }

            return *this;
        }

        constexpr expected(expected&& other) noexcept(
            std::is_nothrow_move_constructible_v<T> &&
            std::is_nothrow_move_constructible_v<E>
        ) requires std::move_constructible<T> && std::move_constructible<E>
            : _is_ok(other._is_ok)
        {
            if (_is_ok)
            {
                std::construct_at(std::addressof(this->_value), std::move(other._value));
            }
            // _value = std::move(other._value);
        }

        constexpr expected(expected&&) noexcept(
            std::is_nothrow_constructible_v<T> &&
            std::is_nothrow_move_constructible_v<T>
        ) requires (
            std::move_constructible<T> &&
            std::move_constructible<E> &&
            std::is_trivially_move_constructible_v<T> &&
            std::is_trivially_move_constructible_v<E>
        ) = default;

        constexpr expected& operator=(expected&& other) noexcept
        {
            if (*this != other)
            {
                _is_ok = other._is_ok;
                _value = std::move(other._value);
            }

            return *this;
        }

        std::string debug_print() const noexcept
        {
            std::string s { "" };
            if (_is_ok)
            {
                s += "<OK>";
            }
            else
            {
                s += "<NOT_OK>";
            }

            return s;
        }

    public:
        [[nodiscard]] constexpr bool has_value() const noexcept { return _is_ok; }
        
        constexpr auto 
        expect(const std::string& message) const & -> T const&
        requires std::is_copy_constructible_v<T>
        {
            // static_assert(std::is_copy_constructible_v<T>, "T must be copyable");
            if (_is_ok)
            {
                return std::get<T>(_value);
            }

            throw new bad_expected_access(message);
        }
        
        constexpr auto 
        expect(const std::string& message) & -> T&
        requires std::is_copy_constructible_v<T>
        {
            // static_assert(std::is_copy_constructible_v<T>, "T must be copyable");
            if (_is_ok)
            {
                return std::get<T>(_value);
            }

            throw new bad_expected_access(message);
        }

        constexpr auto 
        expect(const std::string& message) const & -> T const&&
        requires (std::is_move_constructible_v<T> && std::is_move_assignable_v<T> && !std::is_copy_constructible_v<T>)
        {
            // static_assert(std::is_copy_constructible_v<T>, "T must be copyable");
            if (_is_ok)
            {
                return std::move(std::get<T>(_value));
            }

            throw new bad_expected_access(message);
        }

        constexpr auto
        expect(const std::string& message) & -> T &&
        requires (std::is_move_constructible_v<T> && std::is_move_assignable_v<T> && !std::is_copy_constructible_v<T>)
        {
            // static_assert(std::is_copy_constructible_v<T>, "T must be copyable");
            if (_is_ok)
            {
                return std::move(std::get<T>(_value));
            }

            throw new bad_expected_access();
        }

        constexpr auto
        expect(const std::string& message) const && -> T const&&
        {
            if (_is_ok)
            {
                return std::move(std::get<T>(_value));
            }

            throw new bad_expected_access();
        }

        constexpr auto
        expect(const std::string& message) && -> T &&
        {
            if (_is_ok)
            {
                return std::move(std::get<T>(_value));
            }

            throw new bad_expected_access(message);
        }

        constexpr auto unwrap() const & -> T const&
        requires std::is_copy_constructible_v<T>
        {
            // static_assert(std::is_copy_constructible_v<T>, "T must be copyable");
            if (_is_ok)
            {
                return std::get<T>(_value);
            }

            throw new bad_expected_access();
        }

        constexpr auto unwrap() & -> T&
        requires std::is_copy_constructible_v<T>
        {
            // static_assert(std::is_copy_constructible_v<T>, "T must be copyable");
            if (_is_ok)
            {
                return std::get<T>(_value);
            }

            throw new bad_expected_access();
        }

        constexpr auto unwrap() const & -> T const&&
        requires (std::is_move_constructible_v<T> && std::is_move_assignable_v<T> && !std::is_copy_constructible_v<T>)
        {
            // static_assert(std::is_copy_constructible_v<T>, "T must be copyable");
            if (_is_ok)
            {
                return std::move(std::get<T>(_value));
            }

            throw new bad_expected_access();
        }

        constexpr auto
        unwrap() & -> T &&
        requires (std::is_move_constructible_v<T> && std::is_move_assignable_v<T> && !std::is_copy_constructible_v<T>)
        {
            // static_assert(std::is_copy_constructible_v<T>, "T must be copyable");
            if (_is_ok)
            {
                return std::move(std::get<T>(_value));
            }

            throw new bad_expected_access();
        }

        constexpr auto
        unwrap() const && -> T const&&
        {
            if (_is_ok)
            {
                return std::move(std::get<T>(_value));
            }

            throw new bad_expected_access();
        }

        constexpr auto
        unwrap() && -> T &&
        {
            if (_is_ok)
            {
                return std::move(std::get<T>(_value));
            }

            throw new bad_expected_access();
        }

        [[nodiscard]] constexpr const T& unwrap_or(const T& other) const noexcept
        {
            if (_is_ok)
            {
                return std::get<T>(_value);
            }

            return other;
        }

        [[nodiscard]] constexpr E unwrap_error() const
        {
            if (_is_ok)
            {
                throw new bad_expected_error_access();
            }

            return std::get<E>(_value);
        }
    
    // Operator Overloads
    public:
            auto operator->() -> T*
            {
                if (!_is_ok)
                {
                    throw new bad_expected_access();
                }

                return &std::get<T>(_value);
            }
            
            auto operator->() const -> const T*
            {
                if (!_is_ok)
                {
                    throw new bad_expected_access();
                }

                return &std::get<T>(_value);
            }

    private:
        std::variant<T, E> _value;
        bool _is_ok { false };

        // TODO: should this be more specific with concepts?
        // This is used so that we can do conversions from expected's
        // of different types
        template <typename, typename>
        friend class expected;
    };
}

#endif //RHI_EXPECTED_H
