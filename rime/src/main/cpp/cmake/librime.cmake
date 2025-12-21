set(LIBRIME_SOURCE_DIR ${RIME_JNI_ROOT}/librime)
set(LIBRIME_INCLUDE_DIR ${LIBRIME_SOURCE_DIR}/include)

if(NOT EXISTS ${LIBRIME_SOURCE_DIR})
    message(WARNING "librime source directory not found")
else()
    # Bypass add_subdirectory(${LIBRIME_SOURCE_DIR}),
    # because its find_package() could not find external libraries.
    include(${CMAKE_MODULE_PATH}/librime_CML_Override.cmake)
    if(TARGET rime-static)
        add_library(rime::rime ALIAS rime-static)
        list(APPEND ALL_LINK_LIBS rime-static)
        list(APPEND ALL_INCLUDE_DIRS ${LIBRIME_INCLUDE_DIR})
        list(APPEND ALL_LINK_LIBS rime-static)
        list(APPEND ALL_INCLUDE_DIRS ${LIBRIME_INCLUDE_DIR})
    endif()
endif()