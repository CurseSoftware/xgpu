#ifndef RHI_DATA_WINDOW_H
#define RHI_DATA_WINDOW_H

#include "xgpu/platform.h"

#ifdef RHI_PLATFORM_WINDOWS
// Windows includes
#elif defined(RHI_PLATFORM_LINUX)
// Linux includes
#elif defined(RHI_PLATFORM_APPLE)
// apple includes
#endif // Platform Detection

#include <variant>

namespace xgpu::data
{
    struct HeadlessSurfaceData {};

    struct WindowData 
    {
#ifdef RHI_PLATFORM_WINDOWS
#elif defined(RHI_PLATFORM_LINUX)
#elif defined(RHI_PLATFORM_APPLE)
#endif // Platform Detection
    };

    using SurfaceData = 
        std::variant<
            HeadlessSurfaceData
            , WindowData
        >;
} // namespace xgpu::data

#endif // RHI_DATA_WINDOW_H
