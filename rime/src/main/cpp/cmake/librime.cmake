set(LIBRIME_VERSION "1.15.0")
set(LIBRIME_URL "https://github.com/rime/librime/archive/refs/tags/${LIBRIME_VERSION}.tar.gz")

download_and_extract(
    "librime"
    ${LIBRIME_VERSION}
    ".tar.gz"
    ${LIBRIME_URL}
    "a6283cb6a9fa9445dbd7fac58f614884edd662486fa79809ca63686c8f59c6da"
)

# Do not use add_subdirectory(${LIBRIME_SOURCE_DIR}),
# because its find_package() could not find external libraries.
include(${CMAKE_MODULE_PATH}/librime_cmakelists_override/main.cmake)

add_to_include(librime "${LIBRIME_SOURCE_DIR}/src")
