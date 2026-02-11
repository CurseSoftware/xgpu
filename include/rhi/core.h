#ifndef RHI_CORE_H
#define RHI_CORE_H

#include <cstdint>
namespace xgpu
{
    enum class Preference : std::uint8_t
    {
        NoPreference = 0,
        Preferred = 1,
        Required = 2
    };
} // namespace xgpu

#endif // RHI_CORE_H
