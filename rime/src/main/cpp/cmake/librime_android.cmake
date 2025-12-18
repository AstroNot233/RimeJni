# rime_android_build.cmake
# 独立构建 Android 专用的 librime 静态库，完全绕开原生 CMakeLists.txt 的依赖查找

set(RIME_SOURCE_DIR ${LIBRIME_SOURCE_DIR})
set(RIME_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/librime_build)

# 编译选项（与原项目保持兼容）
set(CMAKE_CXX_STANDARD 17)
set(rime_version 1.15.0)
add_definitions(-DRIME_VERSION="${rime_version}")
add_definitions(-DBOOST_DLL_USE_STD_FS)
add_definitions(-DYAML_CPP_STATIC_DEFINE)
add_definitions(-DOpencc_BUILT_AS_STATIC)
option(ENABLE_THREADING "Enable threading for deployer" ON)
if(NOT ENABLE_THREADING)
  add_definitions(-DRIME_NO_THREADING)
endif()

# 收集核心源文件
file(GLOB_RECURSE RIME_CORE_SOURCES
    ${RIME_SOURCE_DIR}/src/*.cc
    ${RIME_SOURCE_DIR}/src/*.cpp
    ${RIME_SOURCE_DIR}/src/*.c
)
list(FILTER RIME_CORE_SOURCES EXCLUDE REGEX ".*_test\\.(cc|cpp|c)$")

# 收集插件源文件（模拟原项目的 add_subdirectory(plugins)）
set(RIME_PLUGINS_SOURCES "")
file(GLOB_RECURSE PLUGIN_SOURCES
    ${RIME_SOURCE_DIR}/plugins/*/*.cc
    ${RIME_SOURCE_DIR}/plugins/*/*.cpp
    ${RIME_SOURCE_DIR}/plugins/*/*.c
)
list(FILTER PLUGIN_SOURCES EXCLUDE REGEX ".*_test\\.(cc|cpp|c)$")
list(APPEND RIME_PLUGINS_SOURCES ${PLUGIN_SOURCES})

# 创建静态库
add_library(rime-static STATIC
    ${RIME_CORE_SOURCES}
    ${RIME_PLUGINS_SOURCES}
)

target_include_directories(rime-static PRIVATE
    ${RIME_SOURCE_DIR}/include
    ${RIME_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}/librime_build/src  # 用于 build_config.h
)

# Android 必要属性
set_target_properties(rime-static PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# 链接依赖（使用已在上下文中通过 add_subdirectory 定义的目标）
target_link_libraries(rime-static PRIVATE
    Boost::boost
    Boost::filesystem
    Boost::system
    ng-log::ng-log          # 或 glog::glog
    gflags::gflags
    opencc::opencc
    yaml-cpp
    leveldb::leveldb
    marisa::marisa
    Threads::Threads
)

# 创建别名以便其他部分引用
add_library(rime ALIAS rime-static)