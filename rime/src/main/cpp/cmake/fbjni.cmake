set(FBJNI_VERSION "0.7.0")
set(FBJNI_URL "https://github.com/facebookincubator/fbjni/archive/refs/tags/${FBJNI_VERSION}.tar.gz")

download_and_extract(
    "fbjni"
    ${FBJNI_VERSION}
    ".tar.gz"
    ${FBJNI_URL}
    "9e356cc4db48634cc2b3199305c9f1bad396f5844847ff61ba3b3f8080577059"
)

add_subdirectory(${FBJNI_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/fbjni_build EXCLUDE_FROM_ALL)

add_to_include(fbjni "${FBJNI_ROOT}/cxx/fbjni")
