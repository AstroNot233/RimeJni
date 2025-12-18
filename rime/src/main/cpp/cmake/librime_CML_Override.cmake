set(RIME_SOURCE_DIR ${LIBRIME_SOURCE_DIR})
set(RIME_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/librime_build)

set(CMAKE_CXX_STANDARD 17)
set(rime_version 1.15.0)
add_definitions(-DRIME_VERSION="${rime_version}")
add_definitions(-DBOOST_DLL_USE_STD_FS)
add_definitions(-DYAML_CPP_STATIC_DEFINE)
add_definitions(-DOpencc_BUILT_AS_STATIC)

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
    ${OPENCC_ROOT}/src
)

set_target_properties(rime-static PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

find_package(Threads REQUIRED)

set(
    REQUIRED_TARGETS
    "Threads::Threads"
    "Boost::crc"
    "Boost::filesystem"
    "Boost::headers"
    "Boost::interprocess"
    "Boost::regex" 
    "Boost::signals2"
    "Boost::system"
    "Boost::uuid"
    "yaml-cpp"
    "marisa"
    "leveldb"
)

foreach(dep_target ${REQUIRED_TARGETS})
    if(TARGET ${dep_target})
        get_target_property(target_type ${dep_target} TYPE)
        message(STATUS "Target ${dep_target} exists (type: ${dep_target})")
    else()
        message(FATAL_ERROR "Target '${dep_target}' does not exist!")
    endif()
endforeach()

target_link_libraries(rime-static PRIVATE ${REQUIRED_TARGETS})

add_library(rime ALIAS rime-static)
