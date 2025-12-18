set(BOOST_VERSION "1.89.0")
set(BOOST_URL "https://github.com/boostorg/boost/releases/download/boost-${BOOST_VERSION}/boost-${BOOST_VERSION}-cmake.tar.xz")

download_and_extract(
    "Boost"
    ${BOOST_VERSION}
    ".tar.xz"
    ${BOOST_URL}
    "67acec02d0d118b5de9eb441f5fb707b3a1cdd884be00ca24b9a73c995511f74"
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)

add_subdirectory(${BOOST_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/boost_cmake_build EXCLUDE_FROM_ALL)
