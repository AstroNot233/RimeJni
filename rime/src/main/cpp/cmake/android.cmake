message(STATUS "Configuring for Android: ${ANDROID_ABI}")

set(CMAKE_ANDROID_STL_TYPE "c++_shared")

add_compile_options(
    -ffunction-sections
    -fdata-sections
    -fvisibility=hidden
    -fvisibility-inlines-hidden
)

add_link_options(
    -Wl,--gc-sections
    -Wl,--exclude-libs,ALL
)

add_compile_definitions(
    ANDROID
    __ANDROID__
    RIME_ANDROID_BUILD
)