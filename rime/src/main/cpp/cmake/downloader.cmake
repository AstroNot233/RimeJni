function(download_and_extract dep_name version suffix url sha256)
    set(DOWNLOAD_PATH "${RIME_JNI_ROOT}/download")
    set(EXTRACT_PATH "${RIME_JNI_ROOT}")
    set(ARCHIVE_FILE "${DOWNLOAD_PATH}/${dep_name}-${version}${suffix}")

    string(TOUPPER ${dep_name} DEP_NAME)
    string(TOLOWER ${dep_name} dep_name)
    string(TOLOWER ${sha256} sha256)
    
    if(NOT EXISTS "${ARCHIVE_FILE}")
        message(STATUS "Downloading ${dep_name}-${version}${suffix}...")
        file(DOWNLOAD ${url} ${ARCHIVE_FILE} SHOW_PROGRESS TLS_VERIFY ON)
    endif()
    
    file(SHA256 "${ARCHIVE_FILE}" fetched_sha256)
    if(NOT fetched_sha256 STREQUAL sha256)
        file(REMOVE "${ARCHIVE_FILE}")
        message(FATAL_ERROR
            "SHA256 mismatch for ${dep_name}-${version}${suffix}:\n"
            "  Expected: ${sha256}\n"
            "  Got:      ${fetched_sha256}\n"
            "File removed. Please try again.")
    endif()
    
    set(extracted_dir "${EXTRACT_PATH}/${dep_name}-${version}")
    if(NOT EXISTS "${extracted_dir}")
        message(STATUS "Extracting ${dep_name}...")

        set(temp_dir "${EXTRACT_PATH}/temp_${dep_name}_${version}")
        file(ARCHIVE_EXTRACT
            INPUT "${ARCHIVE_FILE}"
            DESTINATION "${temp_dir}")
        file(GLOB extracted_contents "${temp_dir}/*")
        list(LENGTH extracted_contents num_items)

        if(num_items EQUAL 1)
            list(GET extracted_contents 0 actual_dir)
            file(RENAME "${actual_dir}" "${extracted_dir}")
            file(REMOVE_RECURSE "${temp_dir}")
        else()
            file(RENAME "${temp_dir}" "${extracted_dir}")
        endif()
    endif()

    string(REPLACE "-" "_" DEP_NAME_VAR "${DEP_NAME}")
    set(${DEP_NAME_VAR}_ROOT "${extracted_dir}" PARENT_SCOPE)
    set(${DEP_NAME_VAR}_SOURCE_DIR "${extracted_dir}" PARENT_SCOPE)
    message(STATUS "${dep_name} ready at: ${extracted_dir}")
endfunction()