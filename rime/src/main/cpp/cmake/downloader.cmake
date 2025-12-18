set(LIBRIME_CML_PATCH "
cmake_minimum_required(VERSION 3.16)
project(librime LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 1. 直接包含头文件路径，不进行查找
include_directories(
    \${CMAKE_CURRENT_SOURCE_DIR}/include
    \${CMAKE_CURRENT_SOURCE_DIR}/src
    # 假设这些路径通过父项目已经设置好
    \${Boost_INCLUDE_DIRS}
    \${OPENCC_INCLUDE_DIRS}
    \${YAML_CPP_INCLUDE_DIRS}
    \${GLOG_INCLUDE_DIRS}
)

# 2. 收集源文件
file(GLOB_RECURSE LIBRIME_SOURCES
    \"src/*.cc\"
    \"src/*.cpp\"
)

# 3. 创建静态库
add_library(rime STATIC \${LIBRIME_SOURCES})

# 4. 直接链接已知的目标（这些目标必须在父项目中已通过add_subdirectory定义）
target_link_libraries(
    rime
    # Boost 目标 (来自你的add_subdirectory(boost))
    Boost::filesystem
    Boost::system
    # OpenCC 目标 (需确保已通过add_subdirectory引入)
    # 如果OpenCC生成的目标名不同，请替换
    opencc
    # yaml-cpp 目标
    yaml-cpp
    # glog (即我们的ng-log) 目标
    glog::glog
)

# 5. 可选：如果不需要安装，直接移除install部分
if(NOT ANDROID)
    install(TARGETS rime DESTINATION lib)
    install(DIRECTORY include/ DESTINATION include)
endif()
")

