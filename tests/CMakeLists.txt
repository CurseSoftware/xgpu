option(XGPU_ENGINE_BUILD_UNIT_TESTS        "Build unit tests" ON)
option(XGPU_ENGINE_BUILD_INTEGRATION_TESTS "Build integration tests" ON)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0 # or latest version
)

FetchContent_MakeAvailable(Catch2)

if(XGPU_ENGINE_BUILD_UNIT_TESTS)
    add_subdirectory(unit)
endif()

#if(XGPU_ENGINE_BUILD_INTEGRATION_TESTS)
#    add_subdirectory(integration)
#endif()