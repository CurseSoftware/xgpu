cmake_minimum_required(VERSION 3.24)

add_library(xgpu
        "src/api/metal/instance.mm"
        "src/api/metal/adapter.mm"
        "src/api/metal/utils/device.mm"
        "src/api/vulkan/instance.cc"
        "src/api/vulkan/adapter.cc"
        "src/api/vulkan/utils/vulkan_error.cc"
        "src/api/vulkan/utils/extension.cc"
        "src/api/vulkan/utils/validation.cc"
        "src/api/vulkan/utils/debug.cc"
        "src/api/vulkan/utils/device.cc"
)

# Using RTl library
FetchContent_Declare(
        rtl
        GIT_REPOSITORY https://github.com/raustin9/rtl.git
        GIT_TAG main
)
FetchContent_MakeAvailable(rtl)
target_link_libraries(xgpu PUBLIC rtl)

target_include_directories(xgpu
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}"
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

if (APPLE)
    message("Apple platform detected")
    target_link_libraries(xgpu
            PRIVATE
            "-framework Cocoa"
            "-framework Foundation"
            "-framework AppKit"
            "-framework Quartz"
            "-framework Metal"
    )
    target_compile_definitions(xgpu PUBLIC XGPU_COMPILE_METAL)
endif ()

# Vulkan
find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    target_link_libraries(xgpu PUBLIC Vulkan::Vulkan)
    target_compile_definitions(xgpu PUBLIC XGPU_COMPILE_VULKAN)
else ()
    message("VULKAN NOT FOUND")
endif ()

# Add macro for detection on debug builds
target_compile_definitions(xgpu PUBLIC "$<$<CONFIG:Debug>:XGPU_DEBUG>")

set_target_properties(xgpu PROPERTIES
        EXPORT_NAME XGPU::xgpu
        ALIAS_NAME XGPU::XGPU
)

add_library(XGPU::xgpu ALIAS xgpu)
set(XGPU_FOUND TRUE PARENT_SCOPE)