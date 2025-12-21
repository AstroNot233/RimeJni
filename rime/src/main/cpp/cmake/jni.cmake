set(JNI_SOURCE_DIR ${RIME_JNI_ROOT}/jni)

set(JNI_LIB_NAME "rime-jni")

file(GLOB_RECURSE JNI_SOURCES
    ${JNI_SOURCE_DIR}/*.cc
    ${JNI_SOURCE_DIR}/*.cpp
    ${JNI_SOURCE_DIR}/*.hpp
    ${JNI_SOURCE_DIR}/*.c
    ${JNI_SOURCE_DIR}/*.h
)

add_library(${JNI_LIB_NAME} SHARED ${JNI_SOURCES})

set_target_properties(${JNI_LIB_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE ON
)

target_include_directories(${JNI_LIB_NAME} PRIVATE
    ${JNI_SOURCE_DIR}
    ${JNI_INCLUDE_DIR}
)

target_link_libraries(${JNI_LIB_NAME} PRIVATE
    "fbjni"
    "rime-static"
    "c++_static"
)

target_compile_definitions(${JNI_LIB_NAME} PRIVATE
    -DANDROID
    -DANDROID_NDK
    -DLOG_TAG="RimeJNI"
)

target_compile_options(${JNI_LIB_NAME} PRIVATE
    -frtti
    -fexceptions
    -Wall
    -Werror
    -O3
)

target_link_options(${JNI_LIB_NAME} PRIVATE
    -static-libstdc++
    -Wl,--gc-sections
    -Wl,--as-needed
)
