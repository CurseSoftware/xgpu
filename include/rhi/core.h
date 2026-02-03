#ifndef RHI_CORE_H
#define RHI_CORE_H

#include <cstdint>
namespace rhi
{
    enum class Preference : std::uint8_t
    {
        NoPreference = 0,
        Preferred = 1,
        Required = 2
    };
} // namespace rhi

#endif // RHI_CORE_H
