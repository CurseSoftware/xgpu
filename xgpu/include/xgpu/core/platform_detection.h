#pragma once

#if defined(__APPLE__)
#define XGPU_APPLE
#include <TargetConditionals.h>

#if TARGET_OS_IPHONE
#define XGPU_PLATFORM_IOS
#elif defined(TARGET_OS_MAC)
#define XGPU_PLATFORM_MACOS
#endif

#elif defined(WIN32) || defined(__WIN32) || defined(__WIN32__)
#define XGPU_PLATFORM_WIN32
#endif