add_library(MathLib "MathLib.cpp" "MathLib.h" "vector.h")

target_link_libraries(MathLib PUBLIC baseLib)
target_include_directories(MathLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})