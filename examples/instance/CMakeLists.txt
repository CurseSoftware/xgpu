cmake_minimum_required(VERSION 3.10)

project(instance VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable("${PROJECT_NAME}" instance.cc)

target_link_libraries("${PROJECT_NAME}" PRIVATE rhi)
