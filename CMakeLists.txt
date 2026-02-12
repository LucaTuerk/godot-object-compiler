cmake_minimum_required(VERSION 3.10)
project(goc VERSION 0.0 LANGUAGES CXX)
include(CTest)

set(CMAKE_CXX_STANDARD 17)

find_program(MOLD_PROGRAM mold)
if (MOLD_PROGRAM)
    set(CMAKE_LINKER_TYPE MOLD)
endif ()

add_subdirectory(thirdparty)

set(EXEC_NAME "goc")
set(EXEC_NAME_TEST "goc_tests")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE SOURCES_TEST CONFIGURE_DEPENDS "src/library/*.cpp" "src/library_godot/*.cpp" "tests/*.cpp")

add_executable(${EXEC_NAME} ${SOURCES})
add_executable(${EXEC_NAME_TEST} ${SOURCES_TEST})

target_include_directories(${EXEC_NAME} PRIVATE src/ thirdparty/)
target_include_directories(${EXEC_NAME_TEST} PRIVATE src/ thirdparty/ tests/)

target_link_libraries(${EXEC_NAME} PRIVATE
        tree-sitter
        tree-sitter-cpp
)

target_link_libraries(${EXEC_NAME_TEST} PRIVATE
        tree-sitter
        tree-sitter-cpp
)

target_compile_options(${EXEC_NAME} PRIVATE
        $<$<CONFIG:Debug>:-Wall -Wextra -fdiagnostics-color=always -g>
        $<$<CONFIG:Release>:-O3>)

target_precompile_headers(${EXEC_NAME} PRIVATE
        src/library/core/core.h
        src/library/core/file_system_utilities.h
        src/library/core/string_utilities.h
        src/library/core/collection_utilities.h
        src/library/tree/syntax/context.h
)
target_precompile_headers(${EXEC_NAME_TEST} REUSE_FROM ${EXEC_NAME})

target_compile_options(${EXEC_NAME_TEST} PRIVATE
        $<$<CONFIG:Debug>:-Wall -Wextra -fdiagnostics-color=always -g>
        $<$<CONFIG:Release>:-O3>)

add_test(
    NAME GodotObjectCompilerTests
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/goc_tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)