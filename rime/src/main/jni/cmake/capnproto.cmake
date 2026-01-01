set(CAPNPROTO_VERSION "1.3.0")
set(CAPNPROTO_URL "https://github.com/capnproto/capnproto/archive/refs/tags/v${CAPNPROTO_VERSION}.tar.gz")

download_and_extract(
    "capnproto"
    ${CAPNPROTO_VERSION}
    ".tar.gz"
    ${CAPNPROTO_URL}
    "01ab2ba7f52fcc3c51a10e22935aae56f3bc5e99b726b7e507fe6700cb12147d"
)

set(BUILD_TESTING OFF CACHE BOOL "Disable tests" FORCE)
set(CAPNP_BUILD_TESTS OFF CACHE BOOL "Disable capnp tests" FORCE)
set(CAPNP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)

add_subdirectory(${CAPNPROTO_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/capnproto_build)

unset(BUILD_TESTING)
unset(CAPNP_BUILD_TESTS)
unset(CAPNP_BUILD_TOOLS)
