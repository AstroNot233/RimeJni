set(LIBRIME_CMAKE_DIR ${CMAKE_MODULE_PATH}/librime_cmakelists_override)
set(RIME_SOURCE_DIR ${LIBRIME_SOURCE_DIR})
set(RIME_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/librime_build)

set(CMAKE_CXX_STANDARD 17)
set(rime_version 1.15.0)
set(rime_soversion 1)

option(BUILD_SHARED_LIBS "Build Rime as shared library" OFF)
option(BUILD_MERGED_PLUGINS "Merge plugins into one Rime library" ON)
option(BUILD_STATIC "Build with dependencies as static libraries" ON)

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

if(ENABLE_EXTERNAL_PLUGINS)
    list(APPEND $(REQUIRED_TARGETS) dl)
endif()

set(rime_library rime-static)

include(${CMAKE_MODULE_PATH}/librime_cmakelists_override/plugins.cmake)
add_subdirectory(${RIME_SOURCE_DIR}/plugins)

message(STATUS "rime_plugins_libs: ${rime_plugins_deps}")
message(STATUS "rime_plugins_modules: ${rime_plugins_modules}")
set(list "")
foreach(mod ${rime_plugins_modules})
  set(list "${list},Q(${mod})")
endforeach()
set(RIME_SETUP_EXTRA_MODULES "${list}")

include(${CMAKE_MODULE_PATH}/librime_cmakelists_override/src.cmake)
