cmake_minimum_required(VERSION 3.10)
project(goc VERSION 0.0 LANGUAGES CXX)
include(CTest)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(thirdparty)

find_program(MOLD_PROGRAM mold)
if (MOLD_PROGRAM)
    set(CMAKE_LINKER_TYPE MOLD)
endif ()

find_program(CPP_CHECK cppcheck)
if (CPP_CHECK)
    set(CMAKE_CXX_CPPCHECK "cppcheck"
            --suppress=*:*thirdparty/*
    )
endif ()

set(EXEC_NAME "goc")
set(EXEC_NAME_TEST "goc_tests")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE SOURCES_TEST CONFIGURE_DEPENDS "src/application/*.cpp" "src/library/*.cpp" "src/library_godot/*.cpp" "tests/*.cpp")

add_executable(${EXEC_NAME} ${SOURCES})
add_executable(${EXEC_NAME_TEST} ${SOURCES_TEST})

set(BUILD_NUM_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/application/build_number.h)
message(STATUS ${BUILD_NUM_FILE})
add_custom_command(TARGET ${EXEC_NAME}
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -DFILE=${BUILD_NUM_FILE} -P ${CMAKE_CURRENT_LIST_DIR}/tools/automatic_build_number.cmake
)

target_include_directories(${EXEC_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/tree-sitter/lib/include
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/tree-sitter-cpp/bindings/c
)

target_include_directories(${EXEC_NAME_TEST} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src
        ${CMAKE_CURRENT_LIST_DIR}/tests
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/tree-sitter/lib/include
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/tree-sitter-cpp/bindings/c
)

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
        src/library/core/system_includes.h
)

target_precompile_headers(${EXEC_NAME_TEST} REUSE_FROM ${EXEC_NAME})

target_compile_definitions(${EXEC_NAME} PRIVATE DEV_BUILD=1)

target_compile_definitions(${EXEC_NAME_TEST} PRIVATE DEV_BUILD=1)

target_compile_options(${EXEC_NAME_TEST} PRIVATE
        $<$<CONFIG:Debug>:-Wall -Wextra -fdiagnostics-color=always -g>
        $<$<CONFIG:Release>:-O3>)


add_test(
        NAME GodotObjectCompilerTests
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/goc_tests
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)
