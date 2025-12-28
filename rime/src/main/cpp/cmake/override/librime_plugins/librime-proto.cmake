set(PLUGIN_SRC_DIR ${LIBRIME_PROTO_SOURCE_DIR}/src)
file(GLOB PLUGIN_SOURCES ${PLUGIN_SRC_DIR}/src/*.cc)

set(capnp_binary_dir ${LIBRIME_PROTO_SOURCE_DIR}/deps/bin)
set(HOST_CAPNP ${capnp_binary_dir}/capnp.exe)
set(HOST_CAPNPC_CXX ${capnp_binary_dir}/capnpc-c++.exe)

if(NOT (EXISTS ${HOST_CAPNP} AND EXISTS ${HOST_CAPNPC_CXX}))
    message(FATAL_ERROR
        "Required tool not found: ${tool_path}\n"
        "Please download capnproto tools and place in ${LIBRIME_PROTO_SOURCE_DIR}/\n"
        "Download from: https://capnproto.org/install.html\n"
        "Required files: capnp, capnpc-c++")
endif()
set(CAPNPC_OUTPUT_DIR ${LIBRIME_PROTO_SOURCE_DIR}/deps/gen)
set(capnp_gen_srcs
    ${CAPNPC_OUTPUT_DIR}/rime_proto.capnp.c++
    ${CAPNPC_OUTPUT_DIR}/rime_ipc.capnp.c++
)
set(capnp_gen_hdrs
    ${CAPNPC_OUTPUT_DIR}/rime_proto.capnp.h
    ${CAPNPC_OUTPUT_DIR}/rime_ipc.capnp.h
)
add_custom_command(
    OUTPUT ${capnp_gen_srcs} ${capnp_gen_hdrs}
    COMMAND "${HOST_CAPNP}" compile
        -o "${HOST_CAPNPC_CXX}:${CMAKE_CURRENT_BINARY_DIR}"
        --src-prefix "${LIBRIME_PROTO_SOURCE_DIR}"
        -I "${LIBRIME_PROTO_SOURCE_DIR}"
        -I "${CAPNPROTO_ROOT}/c++/src"
        "${LIBRIME_PROTO_SOURCE_DIR}/rime_proto.capnp"
        "${LIBRIME_PROTO_SOURCE_DIR}/rime_ipc.capnp"
    DEPENDS 
        "${LIBRIME_PROTO_SOURCE_DIR}/rime_proto.capnp"
        "${LIBRIME_PROTO_SOURCE_DIR}/rime_ipc.capnp"
    WORKING_DIRECTORY "${LIBRIME_PROTO_SOURCE_DIR}"
    COMMENT "Generating capnp C++ files"
    VERBATIM
)

add_library(rime-proto-objs OBJECT ${capnp_gen_srcs} ${PLUGIN_SOURCES})
add_dependencies(rime-proto-objs capnp_tool capnpc_cpp)

target_include_directories(rime-proto-objs PRIVATE 
    ${CAPNPC_OUTPUT_DIR}
    ${LIBRIME_PROTO_SOURCE_DIR}
)

target_link_libraries(rime-proto-objs PRIVATE capnp capnp-rpc kj)

add_library(rime-proto-deps INTERFACE)
target_link_libraries(rime-proto-deps INTERFACE capnp capnp-rpc kj)

set(plugin_modules "proto")
set(plugin_objs $<TARGET_OBJECTS:rime-proto-objs>)
