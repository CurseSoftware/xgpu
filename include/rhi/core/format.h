#ifndef RHI_CORE_FORMAT_H
#define RHI_CORE_FORMAT_H

#include <format>
namespace rhi::core
{
    template <typename ... Args>
    auto format(const std::format_string<Args...>& fmt, Args&& ... args) -> std::string
    {
        return std::vformat(fmt.get(), std::make_format_args(args...));
    }
} // namespace rhi::core

#endif // RHI_CORE_FORMAT_H
