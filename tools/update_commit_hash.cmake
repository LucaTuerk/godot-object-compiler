function(UpdateCommitHash)
    find_package(Git)
    if (GIT_FOUND)
        execute_process(
                COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
                WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                OUTPUT_VARIABLE COMMIT_HASH
                ERROR_QUIET
                OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    else ()
        message(STATUS "GIT not found")
    endif ()

    file(READ
            ${CMAKE_CURRENT_LIST_DIR}/commit_hash.txt
            COMMIT_HASH_TEMPLATE
    )

    string(REPLACE
            "COMMIT_HASH"
            "${COMMIT_HASH}"
            COMMIT_HASH_CONTENT
            "${COMMIT_HASH_TEMPLATE}"
    )

    file(WRITE
            ${CMAKE_CURRENT_LIST_DIR}/generated/commit_hash.tmp
            "${COMMIT_HASH_CONTENT}"
    )

    execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_LIST_DIR}/generated/commit_hash.tmp ${CMAKE_CURRENT_LIST_DIR}/generated/commit_hash.cpp
    )

    file(REMOVE
            ${CMAKE_CURRENT_LIST_DIR}/generated/commit_hash.tmp
    )
endfunction()

UpdateCommitHash()