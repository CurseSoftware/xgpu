include(FetchContent)
cmake_minimum_required(VERSION 3.24)

project(XGPU VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Macro for defining build options
#macro(xgpu_option var default type docstring)
#    if (NOT DEFINED ${var})
#        set(${var} ${default})
#    endif()
#    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE
#endmacro()

# Add the subdirectory for the actual RHI
add_subdirectory(xgpu)

# Determine if this is a subproject or not
if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(XGPU_SUBPROJECT ON)
endif ()

# TODO: use an option with xgpu_option to conditionally build examples and tests
if (NOT XGPU_SUBPROJECT)
    add_subdirectory(tests)
    # add examples here
endif ()