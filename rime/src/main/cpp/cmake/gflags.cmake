set(GFLAGS_VERSION 2.3.0)
set(GFLAGS_SHA256 f619a51371f41c0ad6837b2a98af9d4643b3371015d873887f7e8d3237320b2f)
set(GFLAGS_ROOT ${RIME_JNI_ROOT}/gflags-${GFLAGS_VERSION})

if(NOT EXISTS "${DOWNLOAD_PATH}/GFLAGS-${GFLAGS_VERSION}.tar.gz")
    message(STATUS "GFLAGS not found. Downloading GFLAGS ${GFLAGS_VERSION}")
    file(
        DOWNLOAD
        "https://github.com/gflags/gflags/archive/refs/tags/v${GFLAGS_VERSION}.tar.gz"
        "${DOWNLOAD_PATH}/GFLAGS-${GFLAGS_VERSION}.tar.gz"
        SHOW_PROGRESS
    )
    message(STATUS "GFLAGS-${GFLAGS_VERSION}.tar.gz downloaded.")
endif()
    
file(SHA256 "${DOWNLOAD_PATH}/GFLAGS-${GFLAGS_VERSION}.tar.gz" GFLAGS_FETCHED_SHA256)
if(NOT GFLAGS_FETCHED_SHA256 STREQUAL GFLAGS_SHA256)
    file(REMOVE_RECURSE "${RIME_JNI_ROOT}/gflags-*")
    file(REMOVE ${DOWNLOAD_PATH}/GFLAGS-${GFLAGS_VERSION}.tar.gz)
    message(WARNING "Get:      ${GFLAGS_FETCHED_SHA256}")
    message(WARNING "Expected: ${GFLAGS_SHA256}")
    message(FATAL_ERROR "Hash mismatch, removing file downloaded.")
endif()

if(NOT EXISTS "${GFLAGS_ROOT}")
    message(STATUS "Extracting GFLAGS ${GFLAGS_VERSION}")
    file(ARCHIVE_EXTRACT INPUT "${DOWNLOAD_PATH}/GFLAGS-${GFLAGS_VERSION}.tar.gz" DESTINATION "${RIME_JNI_ROOT}")
endif()

message(STATUS "Successfully built gflags-${GFLAGS_VERSION}:")
message(STATUS ">>  ${GFLAGS_ROOT}")
