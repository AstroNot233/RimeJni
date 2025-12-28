set(LIBRIME_CMAKE_DIR ${CMAKE_MODULE_PATH}/librime_cmakelists_override)
set(RIME_SOURCE_DIR ${LIBRIME_SOURCE_DIR})
set(RIME_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/librime_build)

set(rime_version 1.15.0)
set(rime_soversion 1)

set(RIME_PLUGINS_DIR "rime-plugins" CACHE STRING "Target directory for externally built Rime plugins")

add_definitions(-DRIME_VERSION="${rime_version}")
add_definitions(-DBOOST_DLL_USE_STD_FS)
add_definitions(-DYAML_CPP_STATIC_DEFINE)
add_definitions(-DOpencc_BUILT_AS_STATIC)

configure_file(
    "${RIME_SOURCE_DIR}/src/rime/build_config.h.in"
    "${RIME_BUILD_DIR}/src/rime/build_config.h"
)

include_directories(${RIME_BUILD_DIR}/src)
include_directories(${RIME_SOURCE_DIR}/src)
include_directories(${RIME_SOURCE_DIR}/include)
link_directories(${RIME_SOURCE_DIR}/lib)

find_package(Threads REQUIRED)
set(BOOST_DEPS "")
foreach(module ${BOOST_MODULES})
    list(APPEND BOOST_DEPS "Boost::${module}")
endforeach()

# keep these variables lest some Rime plugin's cmake file is still using them {
if(NOT DEFINED LIB_INSTALL_DIR)
    set(LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})
endif()

if(NOT DEFINED BIN_INSTALL_DIR)
    set(BIN_INSTALL_DIR ${CMAKE_INSTALL_BINDIR})
endif()
# }

install(
    FILES cmake/RimeConfig.cmake
    DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/cmake/rime
)

file(GLOB rime_public_header_files ${RIME_SOURCE_DIR}/src/*.h)
list(FILTER rime_public_header_files EXCLUDE REGEX .*_impl\.h$)
# install(
#     FILES ${rime_public_header_files}
#     DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}
# )

set(rime_library rime-static)

include(override/librime/plugins)

message(STATUS "rime_plugins_modules: ${rime_plugins_modules}")
set(list "")
foreach(mod ${rime_plugins_modules})
    set(list "${list},Q(${mod})")
endforeach()
set(RIME_SETUP_EXTRA_MODULES "${list}")

include(override/librime/src)
