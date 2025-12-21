cmake_minimum_required(VERSION 3.22)
project(rime-jni)

set(BUILD_TEST OFF)
set(BUILD_STATIC ON)

set(RIME_JNI_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${RIME_JNI_ROOT}/cmake)

include(${CMAKE_MODULE_PATH}/downloader.cmake)

include(${CMAKE_MODULE_PATH}/fbjni.cmake)
include(${CMAKE_MODULE_PATH}/yaml-cpp.cmake)
include(${CMAKE_MODULE_PATH}/OpenCC.cmake)
include(${CMAKE_MODULE_PATH}/LevelDB.cmake)
# include(${CMAKE_MODULE_PATH}/marisa-trie.cmake)
include(${CMAKE_MODULE_PATH}/Boost.cmake)
include(${CMAKE_MODULE_PATH}/librime.cmake)
# include(${CMAKE_MODULE_PATH}/jni.cmake)
