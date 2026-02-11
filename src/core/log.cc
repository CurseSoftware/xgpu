#include "xgpu/core/log.h"

#include <memory>
#include <stdexcept>

namespace xgpu::log
{
    static std::shared_ptr<Logger> g_logger { nullptr };

    auto Logger::get() -> const std::weak_ptr<Logger>
    {
        if (!g_logger)
        {
            // If we are calling this without pre-configuring, then we configure it to be the default
            Logger::init();
        }
        return g_logger;
    }

    auto Logger::init(Config config) -> void
    {
        if (g_logger != nullptr)
        {
            throw std::runtime_error("Cannot reinitialize xgpu::Logger");
        }

        g_logger = std::shared_ptr<Logger>(new Logger(config.out, config.err));
    }

    auto Logger::log(const std::string& message) const noexcept -> void
    {
        _out << message;
    }

    auto Logger::log_err(const std::string& message) const noexcept -> void
    {
        _err << message;
    }
} // namespace xgpu::log
