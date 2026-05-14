#pragma once
#include "concepts.h"
#include "enum_wrapper.h"

#include <cstdint>
#include <format>
#include <source_location>
#include <string>
#include <string_view>

namespace xgpu
{
    /// @brief Enumeration of potential error levels
    enum class ErrorLevel : std::uint32_t
    {
        Warning = 0,
        Error   = 1,
        Fatal   = 2
    };

    template <>
    inline constexpr std::string
    utils::EnumWrapper<ErrorLevel>::to_string() const noexcept
    {
        switch ( value() ) {
        case ErrorLevel::Warning:
            return "WARNING";
        case ErrorLevel::Error:
            return "ERROR";
        case ErrorLevel::Fatal:
            return "FATAL";
        }

        // Should never get here
        return "UNKNOWN_LEVEL";
    }

    template <concepts::Enum E>
    class IError
    {
      public:
        using Code = E;

        /// @brief Make an error from a code and message
        [[nodiscard]] explicit IError(
            const ErrorLevel           level,
            const Code                 code,
            const std::string_view     message  = "",
            const std::source_location location = std::source_location::current()) noexcept
            : m_level{ level }, m_message{ message }, m_code{ code }, m_location{ location }
        {
        }

        /// @brief Create an error with ErrorLevel::Warning
        static IError
        make_warning(
            const Code                 code,
            const std::string_view     message  = "",
            const std::source_location location = std::source_location::current())
        {
            return IError(ErrorLevel::Warning, code, message, location);
        }

        /// @brief Create an error with ErrorLevel::Error
        static IError
        make_error(
            const Code                 code,
            const std::string_view     message  = "",
            const std::source_location location = std::source_location::current())
        {
            return IError(ErrorLevel::Error, code, message, location);
        }

        /// @brief Create an error with ErrorLevel::Fatal
        static IError
        make_fatal(
            const Code                 code,
            const std::string_view     message  = "",
            const std::source_location location = std::source_location::current())
        {
            return IError(ErrorLevel::Fatal, code, message, location);
        }

        /// @brief Get the error message
        [[nodiscard]] std::string_view
        message() const noexcept
        {
            return m_message;
        }

        /// @brief Get the error code
        [[nodiscard]] utils::EnumWrapper<Code>
        code() const noexcept
        {
            return m_code;
        }

        /// @brief Get the error level
        [[nodiscard]] utils::EnumWrapper<ErrorLevel>
        level() const noexcept
        {
            return m_level;
        }

        /// @brief Get the source location
        [[nodiscard]] const std::source_location &
        source_location() const noexcept
        {
            return m_location;
        }

        /// @brief Function to converto to string
        [[nodiscard]] std::string
        to_string() const noexcept
        {
            return std::format(
                "[{}] {}: \"{}\" -> line {} in file: {}", level().to_string(), code().to_string(), message(),
                source_location().line(), source_location().file_name());
        }

        /// @brief Conversion to std::string
        explicit(false) operator std::string() const noexcept { return to_string(); }

        /// @brief Conversion to the error code
        explicit(false) operator Code() const noexcept { return m_code; }

      private:
        utils::EnumWrapper<ErrorLevel> m_level;
        std::string                    m_message;
        utils::EnumWrapper<Code>       m_code;
        std::source_location           m_location;
    };

    // TODO: add actual errors
    enum class ErrorCode
    {
        InstanceError,
    };

    template <>
    inline constexpr std::string
    utils::EnumWrapper<ErrorCode>::to_string() const noexcept
    {
        switch ( m_value ) {
        case ErrorCode::InstanceError:
            return "InstanceError";
        default:
            break;
        }

        return "Unknown";
    }

    using Error = IError<ErrorCode>;
} // namespace xgpu