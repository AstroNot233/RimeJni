set(JNI_SOURCE_DIR ${RIME_JNI_ROOT}/jni)

set(JNI_LIB_NAME "rime-jni")

file(GLOB_RECURSE JNI_SOURCES
    ${JNI_SOURCE_DIR}/*.cc
    ${JNI_SOURCE_DIR}/*.hh
)

add_library(${JNI_LIB_NAME} SHARED
    ${JNI_SOURCES}
)

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
    "rime-static"
    "c++_static"
    "log"
)

target_compile_definitions(${JNI_LIB_NAME} PRIVATE
    -DLOG_TAG="RimeJNI"
)

target_link_options(${JNI_LIB_NAME} PRIVATE
    -Wl,--gc-sections
    -Wl,--as-needed
)
