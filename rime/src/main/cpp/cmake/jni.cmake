set(JNI_SOURCE_DIR ${RIME_JNI_ROOT}/jni)

file(GLOB JNI_SOURCES
    ${JNI_SOURCE_DIR}/*.cpp
    ${JNI_SOURCE_DIR}/*.cc
)

add_library(rime-jni SHARED ${JNI_SOURCES})

set_target_properties(rime-jni PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(rime-jni PRIVATE
    ${JNI_SOURCE_DIR}
    ${FBJNI_ROOT}/cxx/fbjni
    ${LIBRIME_ROOT}/include
    ${LIBRIME_ROOT}/src
)

target_link_libraries(rime-jni PRIVATE
    ${ALL_LINK_LIBS}
)

target_compile_options(rime-jni PRIVATE
    -fexceptions
    -frtti
)
target_link_options(rime-jni PRIVATE
    -Wl,--gc-sections
)