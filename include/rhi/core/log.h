#ifndef RHI_CORE_LOG_H
#define RHI_CORE_LOG_H

#include "rhi/core/format.h"

#include <format>
#include <iostream>
#include <optional>
#include <ostream>
#include <memory>

namespace rhi::log
{
    struct Config
    {
        std::ostream& out { std::cout };
        std::ostream& err { std::cerr };
        bool verbose      { false };
    };

    class Logger
    {
        public:
            static auto init(Config config = Config {}) -> void;
            static auto get() -> const std::weak_ptr<Logger>;

            auto log(const std::string& message) const noexcept -> void;
            auto log_err(const std::string& message) const noexcept -> void;
        
        private:
            [[nodiscard]] explicit Logger(std::ostream& out, std::ostream& err) 
                : _out{ out }
                , _err{ err }
            {}

        private:
            std::ostream& _out;
            std::ostream& _err;
    };

    template <typename ... Args>
    auto info(const std::format_string<Args...>& fmt, Args&& ... args) -> void
    {
        Logger::get().lock()->log(core::format("[INFO]    {}\n", core::format(fmt, args...)));
    }

    template <typename ... Args>
    auto debug(const std::format_string<Args...>& fmt, Args&& ... args) -> void
    {
        Logger::get().lock()->log(core::format("[DEBUG]   {}\n", core::format(fmt, args...)));
    }

    template <typename ... Args>
    auto trace(const std::format_string<Args...>& fmt, Args&& ... args) -> void
    {
        Logger::get().lock()->log(core::format("[TRACE]   {}\n", core::format(fmt, args...)));
    }

    template <typename ... Args>
    auto warning(const std::format_string<Args...>& fmt, Args&& ... args) -> void
    {
        Logger::get().lock()->log(core::format("[WARNING] {}\n", core::format(fmt, args...)));
    }

    template <typename ... Args>
    auto error(const std::format_string<Args...>& fmt, Args&& ... args) -> void
    {
        Logger::get().lock()->log(core::format("[ERROR]   {}\n", core::format(fmt, args...)));
    }

    template <typename ... Args>
    auto fatal(const std::format_string<Args...>& fmt, Args&& ... args) -> void
    {
        Logger::get().lock()->log(core::format("[FATAL]   {}\n", core::format(fmt, args...)));
    }
} // namespace rhi::log

#endif // RHI_CORE_LOG_H
