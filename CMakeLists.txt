cmake_minimum_required(VERSION 3.14)
project(softRenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(application)
add_subdirectory(gpu)
add_subdirectory(glob)
add_subdirectory(Math)


add_executable(softRenderer "main.cpp")

# copy assets into executable file
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMENT "Syncing assets..."
)

target_link_libraries(softRenderer applicationLib gpuLib MathLib)