cmake_minimum_required(VERSION 3.14)
project(softRenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(application)
add_subdirectory(gpu)
add_subdirectory(glob)


add_executable(softRenderer "main.cpp")

target_link_libraries(softRenderer applicationLib gpuLib)