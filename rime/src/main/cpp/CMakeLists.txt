cmake_minimum_required(VERSION 3.22)
project(rime-jni)

set(BUILD_TEST OFF)
set(BUILD_STATIC ON)

set(RIME_JNI_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(DOWNLOAD_PATH ${RIME_JNI_ROOT}/download)
set(CMAKE_MODULE_PATH ${RIME_JNI_ROOT}/cmake)

include(${CMAKE_MODULE_PATH}/fbjni.cmake)
include(${CMAKE_MODULE_PATH}/boost.cmake)
include(${CMAKE_MODULE_PATH}/gflags.cmake)
include(${CMAKE_MODULE_PATH}/nglog.cmake)
include(${CMAKE_MODULE_PATH}/librime.cmake)
include(${CMAKE_MODULE_PATH}/jni.cmake)
