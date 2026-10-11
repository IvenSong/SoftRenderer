add_library(MathLib INTERFACE)

target_link_libraries(MathLib INTERFACE baseLib)
target_include_directories(MathLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
