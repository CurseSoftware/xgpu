#pragma once
#include "xgpu/core/core.h"

#ifdef XGPU_COMPILE_METAL

#ifdef __OBJC__
#include <Metal/Metal.h>

#define OBJC_TYPE(T) T *

#else
#define OBJC_TYPE(T) void *
#endif // __OBJC__

#endif