set(FBJNI_SOURCE_DIR ${RIME_JNI_ROOT}/fbjni)
set(FBJNI_INCLUDE_DIR ${FBJNI_SOURCE_DIR}/include)

if(NOT EXISTS ${FBJNI_SOURCE_DIR}/CMakeLists.txt)
    message(WARNING "fbjni source directory not found or missing CMakeLists.txt")
else()
    add_subdirectory(${FBJNI_SOURCE_DIR})

    if(TARGET fbjni)
        add_library(fbjni::fbjni ALIAS fbjni)
    endif()

    list(APPEND ALL_INCLUDE_DIRS ${FBJNI_INCLUDE_DIR})
    list(APPEND ALL_LINK_LIBS fbjni)
endif()