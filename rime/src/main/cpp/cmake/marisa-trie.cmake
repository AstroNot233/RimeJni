set(MARISA_VERSION "0.3.1")
set(MARISA_URL "https://github.com/s-yata/marisa-trie/archive/refs/tags/v${MARISA_VERSION}.tar.gz")

download_and_extract(
    "marisa-trie"
    ${MARISA_VERSION}
    ".tar.gz"
    ${MARISA_URL}
    "986ed5e2967435e3a3932a8c95980993ae5a196111e377721f0849cad4e807f3"
)

file(GLOB_RECURSE MARISA_SOURCES
    "${MARISA_ROOT}/lib/*.cc"
    "${MARISA_ROOT}/lib/*.cpp"
    "${MARISA_ROOT}/lib/*.c"
)

# 创建静态库
add_library(marisa STATIC ${MARISA_SOURCES_DIR})

target_include_directories(marisa PUBLIC
    ${MARISA_ROOT}/include
    ${MARISA_ROOT}
)

# 可能需要的编译定义
target_compile_definitions(marisa PRIVATE
    MARISA_HAS_STDINT_H
    MARISA_HAS_SYS_TYPES_H
)

set_target_properties(marisa PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_STANDARD 11
)

add_library(marisa::marisa ALIAS marisa)


set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
set(MARISA_BUILD_TESTS OFF CACHE BOOL "")
set(MARISA_BUILD_BENCHMARKS OFF CACHE BOOL "")
set(MARISA_BUILD_SHARED_LIBS OFF CACHE BOOL "")

add_subdirectory(${MARISA_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/MARISA_build EXCLUDE_FROM_ALL)
