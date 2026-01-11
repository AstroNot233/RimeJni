set(LUA_SRC_DIR ${LIBRIME_LUA_SOURCE_DIR}/thirdparty/lua5.4)
set(PLUGIN_SRC_DIR ${LIBRIME_LUA_SOURCE_DIR}/src)

file(GLOB LUA_SOURCES ${LUA_SRC_DIR}/*.c)
set_property(SOURCE ${LUA_SOURCES} PROPERTY COMPILE_DEFINITIONS LUA_USE_POSIX;LUA_USE_DLOPEN)
execute_process(
  COMMAND ${CMAKE_C_COMPILER} -print-multiarch
  OUTPUT_VARIABLE multiarch
  ERROR_QUIET
)
if(multiarch)
  string(REGEX REPLACE "[\r\n]" "" multiarch ${multiarch})
  add_definitions(-DLUA_MULTIARCH=\"${multiarch}\")
  message(STATUS "with multiarch: ${multiarch}")
endif()

file(GLOB PLUGIN_SOURCES
    ${PLUGIN_SRC_DIR}/*.c
    ${PLUGIN_SRC_DIR}/*.cc
    ${PLUGIN_SRC_DIR}/lib/*.c
    ${PLUGIN_SRC_DIR}/lib/*.cc
)

add_library(rime-lua-objs OBJECT
    ${LUA_SOURCES}
    ${PLUGIN_SOURCES}
)

target_include_directories(rime-lua-objs PRIVATE
    ${LUA_SRC_DIR}
    ${PLUGIN_SRC_DIR}
    ${PLUGIN_SRC_DIR}/lib
    ${JNI_INCLUDE_DIR}
    ${JNI_INCLUDE_DIR}/marisa
)

target_compile_definitions(rime-lua-objs PRIVATE
    LUA_COMPAT_5_4
)

target_link_libraries(rime-lua-objs PRIVATE ${BOOST_DEPS} libopencc)

set(plugin_modules "lua")
set(plugin_objs $<TARGET_OBJECTS:rime-lua-objs>)
