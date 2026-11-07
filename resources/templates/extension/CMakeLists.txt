# Using the same minimum as the godot-cpp project
cmake_minimum_required(VERSION 3.17)

# Silence unused variable warning when specified from toolchain
if (CMAKE_C_COMPILER)
endif ()

set(LIBNAME "EXTENSION_NAME") # "The name of the library"
set(GODOTCPP_API_VERSION "GOC_GODOTCPP_API_VERSION")

# Make sure all the dependencies are satisfied
find_package(Python3 3.4 REQUIRED)
find_program(GIT git REQUIRED)

# Ensure godot-cpp submodule has been updated
if (NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/godot-cpp/src")
    message(NOTICE "godot-cpp bindings source not found")
    message(NOTICE "initializing/updating the godot-cpp submodule...")

    # update the c++ bindings submodule to populate it with the necessary source for the library
    execute_process(
            COMMAND git submodule update --init godot-cpp
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
            COMMAND_ERROR_IS_FATAL ANY
    )
endif ()

# Ignore the build directory in the godot editor
file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/.gdignore)

add_subdirectory(godot-cpp SYSTEM)

# Create the gen directory at configure time so IDEs can see generated headers immediately after generate_bindings
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/godot-cpp/gen/include")

# Add godot-cpp's module path and include the exported functions.
# This is made available for documentation generation
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${godot-cpp_SOURCE_DIR}/cmake")
include(GodotCPPModule)

# The godot-cpp target has some of useful properties attached that can be retrieved like so.
get_target_property(GODOTCPP_SUFFIX godot::cpp GODOTCPP_SUFFIX)
get_target_property(GODOTCPP_PLATFORM godot::cpp GODOTCPP_PLATFORM)

# Now we can specify our own project which will inherit any global cmake properties or variables that have been defined.
project(EXTENSION_NAME
        LANGUAGES CXX
)

add_library(${LIBNAME} SHARED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
target_sources(${LIBNAME}
        PRIVATE
        ${SOURCES}
)

target_include_directories(${LIBNAME}
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src
)

include(cmake/autogoc.cmake)
target_autogoc(${LIBNAME} ${CMAKE_CURRENT_LIST_DIR}/src)

# Fetch a list of the xml files to use for documentation and add to our target
file(GLOB_RECURSE DOC_XML LIST_DIRECTORIES NO CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/doc_classes/*.xml")

# conditionally add doc data to compile output
if (DOC_XML)
    if (GODOTCPP_TARGET MATCHES "editor|template_debug")
        target_doc_sources(${LIBNAME} ${DOC_XML})
    endif ()
endif ()

target_link_libraries(${LIBNAME} PRIVATE godot-cpp)

# Require at least C++17 for this target
set_property(TARGET ${LIBNAME} PROPERTY CXX_STANDARD 17)

set(GODOT_PROJECT_BINARY_DIR "${CMAKE_CURRENT_LIST_DIR}/bin/${GODOTCPP_PLATFORM}")

set_target_properties(${LIBNAME}
        PROPERTIES
        # The generator expression here prevents msvc from adding a Debug or Release subdir.
        RUNTIME_OUTPUT_DIRECTORY "$<1:${GODOT_PROJECT_BINARY_DIR}>"

        PREFIX ""
        OUTPUT_NAME "${LIBNAME}${GODOTCPP_SUFFIX}"
)

add_custom_command(TARGET ${LIBNAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${LIBNAME}>" "${GODOT_PROJECT_BINARY_DIR}/$<TARGET_FILE_NAME:${LIBNAME}>"
)
