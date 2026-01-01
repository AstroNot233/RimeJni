set(RIME_PLUGIN_DIRS ${RIME_SOURCE_DIR}/plugins)

# work around CMake build issues on macOS
set(rime_plugin_boilerplate_src "${RIME_PLUGIN_DIRS}/plugin.cc")

set(plugins_module_src "${RIME_PLUGIN_DIRS}/plugins_module.cc")

unset(plugins_objs)
unset(plugins_modules)
unset(plugins_includes)
unset(plugins_src)

add_library(rime-plugins-objs OBJECT ${plugins_module_src})
list(APPEND plugins_objs $<TARGET_OBJECTS:rime-plugins-objs>)
list(APPEND plugins_modules "plugins")

target_include_directories(rime-plugins-objs SYSTEM PRIVATE
    ${JNI_INCLUDE_DIR}
)
target_link_libraries(rime-plugins-objs ${BOOST_DEPS})

file(GLOB plugin_cmakes "${RIME_JNI_ROOT}/cmake/override/librime_plugins/*.cmake")
foreach(plugin ${plugin_cmakes})
    unset(plugin_objs)
    unset(plugin_modules)
    unset(plugin_includes)
    unset(plugin_src)
    include(${plugin})
    list(APPEND plugins_objs ${plugin_objs})
    list(APPEND plugins_modules ${plugin_modules})
    list(APPEND plugins_includes ${plugin_includes})
    list(APPEND plugins_src ${plugin_src})
endforeach(plugin)

set(rime_plugins_objs ${plugins_objs})
set(rime_plugins_modules ${plugins_modules})
set(rime_plugins_src ${plugins_src})

if(EXISTS ${JNI_INCLUDE_DIR}/librime_plugins)
    file(REMOVE_RECURSE ${JNI_INCLUDE_DIR}/librime_plugins)
endif()
file(MAKE_DIRECTORY ${JNI_INCLUDE_DIR}/librime_plugins)

foreach(header ${plugins_includes})
    get_filename_component(name ${header} NAME)
    add_to_include(librime_plugins/${name} ${header})
endforeach()

unset(plugins_objs)
unset(plugins_modules)
unset(plugins_includes)
unset(plugins_src)
