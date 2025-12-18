set(CMAKE_CXX_STANDARD 17)

set(rime_version 1.15.0)
set(rime_soversion 1)

add_definitions(-DRIME_VERSION="${rime_version}")

include(GNUInstallDirs)

option(BUILD_MERGED_PLUGINS "Merge plugins into one Rime library" ON)
option(BUILD_DATA "Build data for Rime" OFF)
option(BUILD_SAMPLE "Build sample Rime plugin" OFF)
option(BUILD_TEST "Build and run tests" ON)
option(BUILD_SEPARATE_LIBS "Build separate rime-* libraries" OFF)
option(INSTALL_PRIVATE_HEADERS "Install private headers (usually needed for externally built Rime plugins)" OFF)
option(ENABLE_EXTERNAL_PLUGINS "Enable loading of externally built Rime plugins (from directory set by RIME_PLUGINS_DIR variable)" OFF)
option(ENABLE_THREADING "Enable threading for deployer" ON)
option(ENABLE_TIMESTAMP "Embed timestamp to schema artifacts" ON)

set(RIME_DATA_DIR "rime-data" CACHE STRING "Target directory for Rime data")
set(RIME_PLUGINS_DIR "rime-plugins" CACHE STRING "Target directory for externally built Rime plugins")

set(Boost_USE_MULTITHREADED ON)

if(NOT TARGET Boost::headers)
  message(WARNING "Boost targets not found. Ensure Boost is added via add_subdirectory.")
else()
  add_definitions(-DBOOST_DLL_USE_STD_FS)
endif()

find_package(Threads)

if(NOT ENABLE_THREADING)
  add_definitions(-DRIME_NO_THREADING)
endif()

if(NOT ENABLE_TIMESTAMP)
  add_definitions(-DRIME_NO_TIMESTAMP)
endif()

if(BUILD_TEST)
  find_package(GTest REQUIRED)
  if(GTEST_FOUND)
    enable_testing()
    include_directories(${GTEST_INCLUDE_DIRS})
  endif()
endif()

find_package(YamlCpp REQUIRED)
if(YamlCpp_FOUND)
  include_directories(${YamlCpp_INCLUDE_PATH})
endif()
if(YamlCpp_STATIC)
  add_definitions(-DYAML_CPP_STATIC_DEFINE)
endif()

find_package(LevelDb REQUIRED)
if(LevelDb_FOUND)
    include_directories(${LevelDb_INCLUDE_PATH})
endif()

find_package(Marisa REQUIRED)
if(Marisa_FOUND)
  include_directories(${Marisa_INCLUDE_PATH})
endif()

find_package(Opencc REQUIRED)
if(Opencc_FOUND)
include_directories(${Opencc_INCLUDE_PATH})
endif()
if(Opencc_STATIC)
  add_definitions(-DOpencc_BUILT_AS_STATIC)
endif()

configure_file(
  "${PROJECT_SOURCE_DIR}/src/rime/build_config.h.in"
  "${PROJECT_BINARY_DIR}/src/rime/build_config.h")

include_directories(${PROJECT_BINARY_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

if (NOT CMAKE_BUILD_PARALLEL_LEVEL)
  include(ProcessorCount)
  ProcessorCount(N)
  if (NOT N EQUAL 0)
    set(CMAKE_BUILD_PARALLEL_LEVEL ${N})
  endif()
endif()

# keep these variables lest some Rime plugin's cmake file is still using them {
if(NOT DEFINED LIB_INSTALL_DIR)
    set(LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})
endif()

if(NOT DEFINED BIN_INSTALL_DIR)
    set(BIN_INSTALL_DIR ${CMAKE_INSTALL_BINDIR})
endif()
# }

# remove target
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)
add_custom_target(remove
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

install(FILES cmake/RimeConfig.cmake
  DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/cmake/rime)

file(GLOB rime_public_header_files ${PROJECT_SOURCE_DIR}/src/*.h)
list(FILTER rime_public_header_files EXCLUDE REGEX .*_impl\.h$)
install(FILES ${rime_public_header_files}
  DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR})
if(INSTALL_PRIVATE_HEADERS)
  file(GLOB rime_private_header_files
    ${PROJECT_SOURCE_DIR}/src/rime/*.h
    ${PROJECT_BINARY_DIR}/src/rime/*.h)
  install(FILES ${rime_private_header_files}
    DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/rime)
  foreach(rime_private_header_files_dir algo config dict gear lever)
    file(GLOB rime_private_header_files
      ${PROJECT_SOURCE_DIR}/src/rime/${rime_private_header_files_dir}/*.h)
    install(FILES ${rime_private_header_files}
      DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/rime/${rime_private_header_files_dir})
  endforeach()
endif()

if(BUILD_DATA)
  file(GLOB rime_preset_data_files ${PROJECT_SOURCE_DIR}/data/preset/*.yaml)
  install(FILES ${rime_preset_data_files} DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/${RIME_DATA_DIR})
endif()

set(rime_library rime-static)

add_subdirectory(plugins)
message(STATUS "rime_plugins_libs: ${rime_plugins_deps}")
message(STATUS "rime_plugins_modules: ${rime_plugins_modules}")
set(list "")
foreach(mod ${rime_plugins_modules})
  set(list "${list},Q(${mod})")
endforeach()
set(RIME_SETUP_EXTRA_MODULES "${list}")

add_subdirectory(src)