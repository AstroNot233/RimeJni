set(RIME_SOURCE_DIR ${LIBRIME_SOURCE_DIR})
set(RIME_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/librime_build)

set(CMAKE_CXX_STANDARD 17)
set(rime_version 1.15.0)
add_definitions(-DRIME_VERSION="${rime_version}")
add_definitions(-DBOOST_DLL_USE_STD_FS)
add_definitions(-DYAML_CPP_STATIC_DEFINE)
add_definitions(-DOpencc_BUILT_AS_STATIC)
option(ENABLE_THREADING "Enable threading for deployer" ON)
if(NOT ENABLE_THREADING)
  add_definitions(-DRIME_NO_THREADING)
endif()

file(GLOB_RECURSE RIME_CORE_SOURCES
    ${RIME_SOURCE_DIR}/src/*.cc
    ${RIME_SOURCE_DIR}/src/*.cpp
    ${RIME_SOURCE_DIR}/src/*.c
)
list(FILTER RIME_CORE_SOURCES EXCLUDE REGEX ".*_test\\.(cc|cpp|c)$")

set(RIME_PLUGINS_SOURCES "")
file(GLOB_RECURSE PLUGIN_SOURCES
    ${RIME_SOURCE_DIR}/plugins/*/*.cc
    ${RIME_SOURCE_DIR}/plugins/*/*.cpp
    ${RIME_SOURCE_DIR}/plugins/*/*.c
)
list(FILTER PLUGIN_SOURCES EXCLUDE REGEX ".*_test\\.(cc|cpp|c)$")
list(APPEND RIME_PLUGINS_SOURCES ${PLUGIN_SOURCES})

add_library(rime-static STATIC
    ${RIME_CORE_SOURCES}
    ${RIME_PLUGINS_SOURCES}
)

target_include_directories(rime-static PRIVATE
    ${RIME_SOURCE_DIR}/include
    ${RIME_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}/librime_build/src
)

set_target_properties(rime-static PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

set(BOOST_LINK_LIBRARIES "")
foreach(lib ${BOOST_LIBS})
    list(APPEND BOOST_LINK_LIBRARIES "Boost::${lib}")
endforeach()

target_link_libraries(rime-static PRIVATE
    ${BOOST_LINK_LIBRARIES}
    # opencc::opencc
    # yaml-cpp
    # leveldb::leveldb
    # marisa::marisa
    # Threads::Threads
)

add_library(rime ALIAS rime-static)