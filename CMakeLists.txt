cmake_minimum_required(VERSION 3.10)
project(goc VERSION 0.0 LANGUAGES CXX)
include(CTest)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(thirdparty)

option(GOC_TESTS_ENABLED "Build the goc_tests executable" OFF)
option(GOC_DEV_BUILD "Build additional goc interal dev utilities" OFF)

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
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

set(EXECUTABLE_OUTPUT_DIR ${CMAKE_BINARY_DIR})
if (WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${EXECUTABLE_OUTPUT_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${EXECUTABLE_OUTPUT_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${EXECUTABLE_OUTPUT_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${EXECUTABLE_OUTPUT_DIR})
endif ()


add_executable(${EXEC_NAME} ${SOURCES})

add_custom_target(
        COMMAND ${CMAKE_COMMAND}
        -T ${EXEC_NAME}
        -P ${CMAKE_CURRENT_LIST_DIR}/tools/update_commit_hash.cmake
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        BYPRODUCTS ${CMAKE_CURRENT_LIST_DIR}/tools/generated/commit_hash.cpp
)

target_sources(${EXEC_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/tools/generated/commit_hash.cpp
)

target_include_directories(${EXEC_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/tree-sitter/lib/include
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/tree-sitter-cpp/bindings/c
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/json/single_include
)

target_link_libraries(${EXEC_NAME} PRIVATE
        tree-sitter
        tree-sitter-cpp
        nlohmann_json
)

if (NOT WIN32)
    target_compile_options(${EXEC_NAME} PRIVATE
            $<$<CONFIG:Debug>:-Wall -Wextra -fdiagnostics-color=always -g>
            $<$<CONFIG:Release>:-O3>)
endif ()

target_precompile_headers(${EXEC_NAME} PRIVATE
        src/library/core/system_includes.h
)

if (GOC_DEV_BUILD)
    target_compile_definitions(${EXEC_NAME} PRIVATE DEV_BUILD=1)
endif ()

if (GOC_TESTS_ENABLED)
    set(EXEC_NAME_TEST "goc_tests")

    file(GLOB_RECURSE SOURCES_TEST CONFIGURE_DEPENDS "src/application/*.cpp" "src/library/*.cpp" "src/library_godot/*.cpp" "tests/*.cpp")

    add_executable(${EXEC_NAME_TEST} ${SOURCES_TEST})

    target_include_directories(${EXEC_NAME_TEST} PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/src
            ${CMAKE_CURRENT_LIST_DIR}/tests
            ${CMAKE_CURRENT_LIST_DIR}/thirdparty/tree-sitter/lib/include
            ${CMAKE_CURRENT_LIST_DIR}/thirdparty/tree-sitter-cpp/bindings/c
            ${CMAKE_CURRENT_LIST_DIR}/thirdparty/json/single_include
    )

    target_sources(${EXEC_NAME_TEST} PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/tools/generated/commit_hash.cpp
    )

    target_link_libraries(${EXEC_NAME_TEST} PRIVATE
            tree-sitter
            tree-sitter-cpp
            nlohmann_json
    )

    target_compile_definitions(${EXEC_NAME_TEST} PRIVATE DEV_BUILD=1)

    if (NOT WIN32)
        target_compile_options(${EXEC_NAME_TEST} PRIVATE
                $<$<CONFIG:Debug>:-Wall -Wextra -fdiagnostics-color=always -g>
                $<$<CONFIG:Release>:-O3>)
    endif ()

    target_precompile_headers(${EXEC_NAME_TEST} REUSE_FROM ${EXEC_NAME})

    add_test(
            NAME GodotObjectCompilerTests
            COMMAND ${CMAKE_CURRENT_BINARY_DIR}/goc_tests
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    )
endif ()
