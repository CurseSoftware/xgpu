
cmake_minimum_required(VERSION 3.10)

project(rhi VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

# Macro for defining build options
macro(rhi_option var default type docstring)
    if (NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
message("Binary output dir: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# CompDB
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# Source files
set(RHI_SOURCES
    "${PROJECT_SOURCE_DIR}/src/instance.cc"
    "${PROJECT_SOURCE_DIR}/src/vk/instance.cc"
)
message("Sources: ${RHI_SOURCES}")

add_library("${PROJECT_NAME}" STATIC "${RHI_SOURCES}")

# Private include path
target_include_directories("${PROJECT_NAME}" PRIVATE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/>
)

# Public include path
target_include_directories("${PROJECT_NAME}" PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

if (WIN32)
    target_compile_definitions("${PROJECT_NAME}" PRIVATE RHI_COMPILE_DX12=1)
endif()

# Look for vulkan and link if available
find_package(Vulkan QUIET)
if (Vulkan_FOUND)
    message("Vulkan found.")
    target_link_libraries("${PROJECT_NAME}" PRIVATE Vulkan::Vulkan)
    target_compile_definitions("${PROJECT_NAME}" PUBLIC RHI_COMPILE_VULKAN_BACKEND=1)
else()
    message("Vulkan not found.")
endif()

rhi_option(RHI_BUILD_EXAMPLES ON BOOL "ON to build examples. OFF to ignore them")
if (RHI_BUILD_EXAMPLES)
    message("Building examples")
    add_subdirectory(examples/instance)
endif()
