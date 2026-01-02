cmake_minimum_required(VERSION 3.22)
project(rime-jni)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_TEST OFF)
set(BUILD_STATIC ON)

set(RIME_JNI_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND CMAKE_MODULE_PATH ${RIME_JNI_ROOT}/cmake)

set(DOWNLOAD_PATH "${RIME_JNI_ROOT}/Downloads")
if(NOT EXISTS ${DOWNLOAD_PATH})
    file(MAKE_DIRECTORY ${DOWNLOAD_PATH})
endif()

set(JNI_DEPS_DIR ${RIME_JNI_ROOT}/deps)
if(NOT EXISTS ${JNI_DEPS_DIR})
    file(MAKE_DIRECTORY ${JNI_DEPS_DIR})
endif()

set(JNI_INCLUDE_DIR ${RIME_JNI_ROOT}/include)
if(NOT EXISTS ${JNI_INCLUDE_DIR})
    file(MAKE_DIRECTORY ${JNI_INCLUDE_DIR})
endif()

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

include(Helper)

include(Boost)
include(LevelDB)
include(yaml-cpp)
include(marisa-trie)
include(OpenCC)
include(librime)

include(jni)
