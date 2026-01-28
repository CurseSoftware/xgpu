#ifndef RHI_DATA_SURFACE_H
#define RHI_DATA_SURFACE_H
#include "rhi/platform.h"

#ifdef RHI_PLATFORM_WINDOWS

namespace rhi::data
{
    struct Win32WindowData
    {
    };
} // namespace rhi::data

#endif // RHI_PLATFORM_WINDOWS
#endif // RHI_DATA_SURFACE_H
