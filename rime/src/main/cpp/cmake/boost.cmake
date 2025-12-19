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

set(BOOST_MODULES
    "algorithm"
    "filesystem"
    "system"
    "regex"
    "interprocess"
    "signals2"
    "uuid"
    "crc"
    "range"
    "scope_exit"
)
set(BOOST_INCLUDE_DIRS "")

foreach(module ${BOOST_MODULES})
    set(module_path "${BOOST_ROOT}/libs/${module}/include")
    if(EXISTS "${module_path}")
        list(APPEND BOOST_INCLUDE_DIRS "${module_path}")
    else()
        message(WARNING "Boost module ${module} not found at ${module_path}")
    endif()
endforeach()

target_include_directories(boost_headers SYSTEM INTERFACE ${BOOST_INCLUDE_DIRS})
