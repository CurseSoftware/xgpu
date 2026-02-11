#ifndef RHI_CORE_FORMAT_H
#define RHI_CORE_FORMAT_H

#include <format>
namespace xgpu::core
{
    template <typename ... Args>
    auto format(const std::format_string<Args...> fmt, Args&& ... args) -> std::string
    {
        return std::vformat(fmt.get(), std::make_format_args(args...));
    }
} // namespace xgpu::core

#endif // RHI_CORE_FORMAT_H
