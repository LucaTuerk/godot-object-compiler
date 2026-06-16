cmake_minimum_required(VERSION 3.22.0)
project(goc VERSION 0.0 LANGUAGES CXX)
include(CTest)
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(thirdparty)

option(GOC_TREE_SITTER_PARSER_ENABLED "Build the treesitter based source parser" ON)
option(GOC_LIBCLANG_PARSER_ENABLED "Build the libclang based source parser" ON)
option(GOC_TESTS_ENABLED "Build the goc_tests executable" OFF)
option(GOC_DEV_BUILD "Build additional goc interal dev utilities" OFF)

find_program(MOLD_PROGRAM mold)
if (MOLD_PROGRAM)
    set(CMAKE_LINKER_TYPE MOLD)
endif ()

find_program(CPP_CHECK cppcheck)
if (CPP_CHECK)
    if (PROJECT_IS_TOPLEVEL)
        set(CMAKE_CXX_CPPCHECK "cppcheck"
                --suppress=*:*thirdparty/*
                --suppress=nullPointer:src/*
        )
    endif ()
endif ()

if (${GOC_LIBCLANG_PARSER_ENABLED})
    find_package(Clang CONFIG REQUIRED)
endif ()

set(EXEC_NAME "goc")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


file(GLOB_RECURSE APPLICATION_SOURCES CONFIGURE_DEPENDS "src/application/*.cpp")
file(GLOB_RECURSE TREESITTER_PARSER_SOURCES CONFIGURE_DEPENDS "src/library/parsers/tree-sitter/*.cpp")
file(GLOB_RECURSE LIBCLANG_PARSER_SOURCES CONFIGURE_DEPENDS "src/library/parsers/libclang/*.cpp")

file(GLOB_RECURSE LIBRARY_GODOT_SOURCES CONFIGURE_DEPENDS "src/library_godot/*.cpp")
file(GLOB_RECURSE LIBRARY_SOURCES CONFIGURE_DEPENDS "src/library/*.cpp")
list(FILTER LIBRARY_SOURCES EXCLUDE REGEX "src/library/parsers/tree-sitter")
list(FILTER LIBRARY_SOURCES EXCLUDE REGEX "src/library/parsers/libclang")
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

set(EXECUTABLE_OUTPUT_DIR ${CMAKE_BINARY_DIR})
if (WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${EXECUTABLE_OUTPUT_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${EXECUTABLE_OUTPUT_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${EXECUTABLE_OUTPUT_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${EXECUTABLE_OUTPUT_DIR})
endif ()


add_executable(${EXEC_NAME}
        src/main.cpp
        ${LIBRARY_SOURCES}
        ${LIBRARY_GODOT_SOURCES}
        ${APPLICATION_SOURCES}
        $<$<BOOL:${GOC_LIBCLANG_PARSER_ENABLED}>:${TREESITTER_PARSER_SOURCES}>
        $<$<BOOL:${GOC_TREE_SITTER_PARSER_ENABLED}>:${LIBCLANG_PARSER_SOURCES}>
)

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
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/json/single_include
        $<$<BOOL:${GOC_TREE_SITTER_PARSER_ENABLED}>:
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/tree-sitter/lib/include
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/tree-sitter-cpp/bindings/c
        >
        $<$<BOOL:${GOC_LIBCLANG_PARSER_ENABLED}>:
        ${CLANG_INCLUDE_DIRS}
        >
)

target_link_libraries(${EXEC_NAME} PRIVATE
        nlohmann_json
        $<$<BOOL:${GOC_TREE_SITTER_PARSER_ENABLED}>:
        tree-sitter
        tree-sitter-cpp
        >
        $<$<BOOL:${GOC_LIBCLANG_PARSER_ENABLED}>:
        libclang
        >
)


if (NOT WIN32)
    target_compile_options(${EXEC_NAME} PRIVATE
            $<$<CONFIG:Debug>:-Wall -Wextra -fdiagnostics-color=always -g>
            $<$<CONFIG:Release>:-O3>)
endif ()

target_precompile_headers(${EXEC_NAME} PRIVATE
        src/library/core/system_includes.h
)

target_compile_definitions(${EXEC_NAME} PRIVATE
        $<$<BOOL:${GOC_DEV_BUILD}>:DEV_BUILD=1>
        $<$<BOOL:${GOC_LIBCLANG_PARSER_ENABLED}>:GOC_LIBCLANG_PARSER_ENABLED=1>
        $<$<BOOL:${GOC_TREE_SITTER_PARSER_ENABLED}>:GOC_TREE_SITTER_PARSER_ENABLED=1>
)

if (GOC_TESTS_ENABLED)
    set(EXEC_NAME_TEST "goc_tests")

    file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "tests/*.cpp")

    add_executable(${EXEC_NAME_TEST}
            ${TEST_SOURCES}
            ${LIBRARY_SOURCES}
            ${LIBRARY_GODOT_SOURCES}
            ${APPLICATION_SOURCES}
            $<$<BOOL:${GOC_TREE_SITTER_PARSER_ENABLED}>:${TREESITTER_PARSER_SOURCES}>
            $<$<BOOL:${GOC_LIBCLANG_PARSER_ENABLED}>:${LIBCLANG_PARSER_SOURCES}>
    )

    target_include_directories(${EXEC_NAME_TEST} PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/src
            ${CMAKE_CURRENT_LIST_DIR}/tests
            ${CMAKE_CURRENT_LIST_DIR}/thirdparty/json/single_include
            $<$<BOOL:${GOC_TREE_SITTER_PARSER_ENABLED}>:
            ${CMAKE_CURRENT_LIST_DIR}/thirdparty/tree-sitter/lib/include
            ${CMAKE_CURRENT_LIST_DIR}/thirdparty/tree-sitter-cpp/bindings/c
            >
            $<$<BOOL:${GOC_LIBCLANG_PARSER_ENABLED}>:
            ${CLANG_INCLUDE_DIRS}
            >
    )

    target_sources(${EXEC_NAME_TEST} PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/tools/generated/commit_hash.cpp
    )

    target_link_libraries(${EXEC_NAME_TEST} PRIVATE
            nlohmann_json
            $<$<BOOL:${GOC_TREE_SITTER_PARSER_ENABLED}>:
            tree-sitter
            tree-sitter-cpp
            >
            $<$<BOOL:${GOC_LIBCLANG_PARSER_ENABLED}>:
            libclang
            >
    )

    target_compile_definitions(${EXEC_NAME_TEST} PRIVATE
            DEV_BUILD=1
            $<$<BOOL:${GOC_LIBCLANG_PARSER_ENABLED}>:GOC_LIBCLANG_PARSER_ENABLED=1>
            $<$<BOOL:${GOC_TREE_SITTER_PARSER_ENABLED}>:GOC_TREE_SITTER_PARSER_ENABLED=1>
    )

    if (NOT WIN32)
        target_compile_options(${EXEC_NAME_TEST} PRIVATE
                -Wall -Wextra -fdiagnostics-color=always -coverage -g
        )
        target_link_options(${EXEC_NAME_TEST} PRIVATE -coverage)
    endif ()

    target_precompile_headers(${EXEC_NAME_TEST} PRIVATE
            src/library/core/system_includes.h
    )

    add_test(
            NAME GodotObjectCompilerTests
            COMMAND ${CMAKE_CURRENT_BINARY_DIR}/goc_tests
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    )
endif ()
