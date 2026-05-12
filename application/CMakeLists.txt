add_library(applicationLib "application.cpp" "application.h" "image.h" "image.cpp")

target_link_libraries(applicationLib PUBLIC baseLib)
target_include_directories(applicationLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})