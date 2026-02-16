
function(AUTOGOC TARGET ROOT_DIR)
    get_target_property(SOURCE_DIR ${TARGET} SOURCE_DIR)
    get_target_property(BINARY_DIR ${TARGET} BINARY_DIR)
    get_target_property(SOURCES ${TARGET} SOURCES)
    get_target_property(INCLUDE_DIRECTORIES ${TARGET} INCLUDE_DIRECTORIES)

    if (TARGET godot-cpp)
        get_target_property(GodotCPP_INCLUDE_DIRECTORIES godot-cpp INCLUDE_DIRECTORIES)
        list(PREPEND INCLUDE_DIRECTORIES ${GodotCPP_INCLUDE_DIRECTORIES})
    endif ()

    list(JOIN INCLUDE_DIRECTORIES "," INCLUDE_JOINED)
    list(JOIN SOURCES "," SOURCES_JOINED)

    if (TARGET goc)
        get_target_property(GOC_BINARY_DIR goc BINARY_DIR)
        message(STATUS "AUTOGOC: Using goc target executable ${GOC_BINARY_DIR}/goc")

        add_custom_command(
                OUTPUT bindings
                COMMAND ${GOC_BINARY_DIR}/goc generate -R=${ROOT_DIR} -P=.goc -C=.goc/cache -G=.goc/generated -I=${INCLUDE_JOINED} -S=${SOURCES_JOINED}
                WORKING_DIRECTORY ${BINARY_DIR}
                DEPENDS goc godot-cpp generate_bindings
        )
    elseif (DEFINED ENV{GOC_EXECUTABLE})
        message(STATUS "AUTOGOC: Using goc executable ${GOC_EXECUTABLE}")
        add_custom_command(
                OUTPUT bindings
                COMMAND $ENV{GOC_EXECUTABLE} generate -R=${ROOT_DIR} -P=.goc -C=.goc/cache -G=.goc/generated -I=${INCLUDE_JOINED} -S=${SOURCES_JOINED}
                WORKING_DIRECTORY ${BINARY_DIR}
                DEPENDS godot-cpp generate_bindings
        )
    else ()
        message(FATAL_ERROR "AUTOGOC: goc executable not found\n"
                "Please set GOC_EXECUTABLE environment variable to the path of the goc executable\n"
                "If you have the goc repository cloned, you can alternatively add the target subdirectory using add_subdirectory(<path_to_goc_repository>).\n"
        )
    endif ()

    add_custom_target(goc_generated_files DEPENDS bindings)
    add_dependencies(${TARGET} goc_generated_files)

    target_include_directories(${TARGET} PRIVATE
            ${BINARY_DIR}/.goc/generated
    )
    file(GLOB_RECURSE GENERATED_SOURCES CONFIGURE_DEPENDS "${BINARY_DIR}/.goc/generated/*.cpp")
    target_sources(${TARGET}
            PRIVATE
            ${GENERATED_SOURCES}
    )
endfunction()
