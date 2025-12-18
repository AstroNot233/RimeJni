set(NG_LOG_VERSION 0.8.2)
set(NG_LOG_SHA256 4d7467025b800828d3b2eb87eb506b310d090171788857601a708a46825953a8)
set(NG_LOG_ROOT ${RIME_JNI_ROOT}/ng-log-${NG_LOG_VERSION})

if(NOT EXISTS "${DOWNLOAD_PATH}/ng_log-${NG_LOG_VERSION}.tar.gz")
    message(STATUS "ng_log not found. Downloading ng_log ${NG_LOG_VERSION}")
    file(
        DOWNLOAD
        "https://codeload.github.com/ng-log/ng-log/tar.gz/refs/tags/v${NG_LOG_VERSION}"
        "${DOWNLOAD_PATH}/ng_log-${NG_LOG_VERSION}.tar.gz"
        SHOW_PROGRESS
    )
    message(STATUS "ng_log-${NG_LOG_VERSION}.tar.gz downloaded.")
endif()
    
file(SHA256 "${DOWNLOAD_PATH}/ng_log-${NG_LOG_VERSION}.tar.gz" NG_LOG_FETCHED_SHA256)
if(NOT NG_LOG_FETCHED_SHA256 STREQUAL NG_LOG_SHA256)
    file(REMOVE_RECURSE "${RIME_JNI_ROOT}/ng-log-*")
    file(REMOVE ${DOWNLOAD_PATH}/ng_log-${NG_LOG_VERSION}.tar.gz)
    message(WARNING "Get:      ${NG_LOG_FETCHED_SHA256}")
    message(WARNING "Expected: ${NG_LOG_SHA256}")
    message(FATAL_ERROR "Hash mismatch, removing file downloaded.")
endif()

if(NOT EXISTS "${NG_LOG_ROOT}")
    message(STATUS "Extracting ng_log ${NG_LOG_VERSION}")
    file(ARCHIVE_EXTRACT INPUT "${DOWNLOAD_PATH}/ng_log-${NG_LOG_VERSION}.tar.gz" DESTINATION "${RIME_JNI_ROOT}")
endif()

message(STATUS "Successfully built ng_log-${NG_LOG_VERSION}:")
message(STATUS ">>  ${NG_LOG_ROOT}")
