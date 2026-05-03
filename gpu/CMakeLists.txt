add_library(gpuLib "gpu.cpp" "gpu.h" "frameBuffer.cpp" "frameBuffer.h" "raster.cpp" "raster.h")

target_link_libraries(gpuLib PUBLIC baseLib)
# 【进阶技巧】告诉 CMake：任何链接了 funcs 的目标，都可以自动把当前目录作为头文件搜索路径
target_include_directories(gpuLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})