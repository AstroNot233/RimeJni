set(BOOST_VERSION 1.89.0)
set(BOOST_SHA256 875cc413afa6b86922b6df3b2ad23dec4511c8a741753e57c1129e7fa753d700)
set(BOOST_ROOT ${RIME_JNI_ROOT}/boost-${BOOST_VERSION})

if(NOT EXISTS "${DOWNLOAD_PATH}/boost-${BOOST_VERSION}.tar.xz")
    message(STATUS "Boost not found. Downloading Boost ${BOOST_VERSION}")
    file(
        DOWNLOAD
        "https://github.com/boostorg/boost/releases/download/boost-${BOOST_VERSION}/boost-${BOOST_VERSION}-b2-nodocs.tar.xz"
        "${DOWNLOAD_PATH}/boost-${BOOST_VERSION}.tar.xz"
        SHOW_PROGRESS
    )
    message(STATUS "boost-${BOOST_VERSION}.tar.xz downloaded.")
endif()
    
file(SHA256 "${DOWNLOAD_PATH}/boost-${BOOST_VERSION}.tar.xz" BOOST_FETCHED_SHA256)
if(NOT BOOST_FETCHED_SHA256 STREQUAL BOOST_SHA256)
    file(REMOVE_RECURSE "${RIME_JNI_ROOT}/boost-*")
    file(REMOVE ${DOWNLOAD_PATH}/boost-${BOOST_VERSION}.tar.xz)
    message(WARNING "Get:      ${BOOST_FETCHED_SHA256}")
    message(WARNING "Expected: ${BOOST_SHA256}")
    message(FATAL_ERROR "Hash mismatch, removing file downloaded.")
endif()

if(NOT EXISTS "${BOOST_ROOT}")
    message(STATUS "Extracting Boost ${BOOST_VERSION}")
    file(ARCHIVE_EXTRACT INPUT "${DOWNLOAD_PATH}/boost-${BOOST_VERSION}.tar.xz" DESTINATION "${RIME_JNI_ROOT}")
endif()

include(ExternalProject)
add_custom_target(boost_build
    COMMAND ${BOOST_ROOT}/bootstrap.sh
    COMMAND ${BOOST_ROOT}/b2 toolset=clang target-os=android --prefix=${CMAKE_BINARY_DIR}/boost_install install
    WORKING_DIRECTORY ${BOOST_ROOT}
)

message(STATUS "Successfully built boost-${BOOST_VERSION}:")
message(STATUS ">>  ${BOOST_ROOT}")
