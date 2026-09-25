add_library(gpuLib "gpu.cpp" "gpu.h" "frameBuffer.cpp" "frameBuffer.h" "raster.cpp" "raster.h" "dataStructure.h" "BufferObject.h" "BufferObject.cpp" "VAO.h" "VAO.cpp" "shader.cpp" "shader.h" "Shader/defaultShader.h" "Shader/defaultShader.cpp" "clipper.h" "clipper.cpp" "texture.h"  "texture.cpp" "Shader/textureShader.h" "Shader/textureShader.cpp" "camera.h" "camera.cpp")

target_link_libraries(gpuLib PUBLIC baseLib)
target_link_libraries(gpuLib PUBLIC MathLib)
target_link_libraries(gpuLib PUBLIC applicationLib)
target_include_directories(gpuLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})