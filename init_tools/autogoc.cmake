
function(AUTOGOC TARGET)
    get_target_property(SOURCE_DIR ${TARGET} SOURCE_DIR)

    if(TARGET goc)
        get_target_property(GOC_BINARY_DIR goc BINARY_DIR)
        message(STATUS "AUTOGOC: Using goc target executable ${GOC_BINARY_DIR}/goc")

        add_custom_command(
            OUTPUT ${SOURCE_DIR}/.goc/generated/macros.h
            COMMAND ${GOC_BINARY_DIR}/goc generate
            WORKING_DIRECTORY ${SOURCE_DIR}
            DEPENDS goc
        )
    elseif(DEFINED ENV{GOC_EXECUTABLE})
        message(STATUS "AUTOGOC: Using goc executable ${GOC_EXECUTABLE}")
        add_custom_command(
            OUTPUT ${SOURCE_DIR}/.goc/generated/macros.h
            COMMAND $ENV{GOC_EXECUTABLE} generate
            WORKING_DIRECTORY ${SOURCE_DIR}
        )
    else()
        message(FATAL_ERROR "AUTOGOC: goc executable not found\n"
            "Please set GOC_EXECUTABLE environment variable to the path of the goc executable\n"
            "If you have the goc repository cloned, you can alternatively add the target subdirectory using add_subdirectory(<path_to_goc_repository>).\n"
        )
    endif()

    add_custom_target(goc_generated_files DEPENDS ${SOURCE_DIR}/.goc/generated/macros.h)
    add_dependencies(${TARGET} goc_generated_files)
endfunction()
