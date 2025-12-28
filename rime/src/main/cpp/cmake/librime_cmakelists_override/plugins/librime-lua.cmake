set(plugin_modules "lua")
set(plugin_objs $<TARGET_OBJECTS:rime-lua-objs>)
set(LUA_SRC_DIR ${LIBRIME_LUA_SOURCE_DIR}/thirdparty/lua5.4)
set(PLUGIN_SRC_DIR ${LIBRIME_LUA_SOURCE_DIR}/src)

file(GLOB LUA_SOURCES ${LUA_SRC_DIR}/*.c)
file(GLOB PLUGIN_SOURCES ${PLUGIN_SRC_DIR}/*.cc ${PLUGIN_SRC_DIR}/lib/*.cc)

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
    LUA_COMPAT_5_3
)

target_link_libraries(rime-lua-objs PRIVATE ${BOOST_DEPS})
