cmake_minimum_required(VERSION 3.14)
project(softRenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Assimp is supplied as MSVC Debug and Release builds in thirdParty. Its
# exported CMake package still points to the original install layout.
if(NOT MSVC)
    message(FATAL_ERROR "The Assimp binaries in thirdParty require MSVC.")
endif()

set(ASSIMP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty")
set(ASSIMP_INCLUDE_DIR "${ASSIMP_ROOT}/include")
set(ASSIMP_DEBUG_IMPORT_LIBRARY "${ASSIMP_ROOT}/lib/assimp/assimp-vc143-mtd.lib")
set(ASSIMP_DEBUG_RUNTIME_LIBRARY "${ASSIMP_ROOT}/bin/assimp/assimp-vc143-mtd.dll")
set(ASSIMP_RELEASE_IMPORT_LIBRARY "${ASSIMP_ROOT}/lib/assimp/assimp-vc143-mt.lib")
set(ASSIMP_RELEASE_RUNTIME_LIBRARY "${ASSIMP_ROOT}/bin/assimp/assimp-vc143-mt.dll")
foreach(ASSIMP_PATH IN ITEMS "${ASSIMP_INCLUDE_DIR}"
        "${ASSIMP_DEBUG_IMPORT_LIBRARY}" "${ASSIMP_DEBUG_RUNTIME_LIBRARY}"
        "${ASSIMP_RELEASE_IMPORT_LIBRARY}" "${ASSIMP_RELEASE_RUNTIME_LIBRARY}")
    if(NOT EXISTS "${ASSIMP_PATH}")
        message(FATAL_ERROR "Missing Assimp file or directory: ${ASSIMP_PATH}")
    endif()
endforeach()

add_library(assimp::assimp SHARED IMPORTED GLOBAL)
set_target_properties(assimp::assimp PROPERTIES
    IMPORTED_CONFIGURATIONS "DEBUG;RELEASE"
    IMPORTED_IMPLIB_DEBUG "${ASSIMP_DEBUG_IMPORT_LIBRARY}"
    IMPORTED_LOCATION_DEBUG "${ASSIMP_DEBUG_RUNTIME_LIBRARY}"
    IMPORTED_IMPLIB_RELEASE "${ASSIMP_RELEASE_IMPORT_LIBRARY}"
    IMPORTED_LOCATION_RELEASE "${ASSIMP_RELEASE_RUNTIME_LIBRARY}"
    MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE
    MAP_IMPORTED_CONFIG_MINSIZEREL RELEASE
    INTERFACE_INCLUDE_DIRECTORIES "${ASSIMP_INCLUDE_DIR}"
)

function(target_use_assimp target)
    target_link_libraries(${target} PRIVATE assimp::assimp)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:assimp::assimp>" "$<TARGET_FILE_DIR:${target}>"
        COMMENT "Copying Assimp runtime for ${target}"
    )
endfunction()


add_subdirectory(glob)
add_subdirectory(Math)
add_subdirectory(application)
add_subdirectory(gpu)


add_executable(softRenderer "main.cpp" "prepare.cpp" "prepare.h" "cubeMesh.h")

# copy assets into executable file
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMENT "Syncing assets..."
)

target_link_libraries(softRenderer PRIVATE applicationLib gpuLib MathLib)
target_use_assimp(softRenderer)

add_subdirectory(experiments)
