#ifndef RHI_DATA_SURFACE_INFO_H
#define RHI_DATA_SURFACE_INFO_H

#include "rhi/platform.h"
#include <X11/X.h>

#if defined(RHI_PLATFORM_LINUX)
#include <X11/Xlib.h>
#elif defined(RHI_PLATFORM_WINDOWS)
#elif defined(RHI_PLATFORM_APPLE)
#endif // Platform Detection

namespace rhi::data
{
    struct SurfaceInfo
    {
#if defined(RHI_PLATFORM_LINUX)
        Display* display;
        Window window;
#elif defined(RHI_PLATFORM_WINDOWS)
#elif defined(RHI_PLATFORM_APPLE)
#endif // Platform Detection
    };
} // namespace rhi::data

#endif // RHI_DATA_SURFACE_INFO_H
