add_executable(unit_tests
        "instance.cc"
        "error.cc"
        "flag.cc"
        "vulkan/instance.cc"
)


target_link_libraries(unit_tests PRIVATE xgpu Catch2::Catch2WithMain)