add_executable(cubeLightingExperiment "cube_lighting.cpp" "../prepare.cpp")
target_link_libraries(cubeLightingExperiment PRIVATE gpuLib)
target_use_assimp(cubeLightingExperiment)

add_executable(modelImportExperiment "model_import.cpp" "../prepare.cpp")
target_link_libraries(modelImportExperiment PRIVATE gpuLib)
target_use_assimp(modelImportExperiment)
