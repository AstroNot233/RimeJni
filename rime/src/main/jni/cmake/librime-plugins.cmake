function(add_to_plugins dep_name src_path)
    string(TOLOWER ${dep_name} dep_name)

    if(EXISTS ${LIBRIME_SOURCE_DIR}/plugins/${dep_name})
        file(REMOVE ${LIBRIME_SOURCE_DIR}/plugins/${dep_name})
    endif()
    file(CREATE_LINK
        ${src_path}
        ${LIBRIME_SOURCE_DIR}/plugins/${dep_name}
        SYMBOLIC
    )
endfunction()

download_and_extract(
    "librime-lua"
    "master"
    ".zip"
    "https://github.com/hchunhui/librime-lua/archive/refs/heads/master.zip"
    "bypass"
)
add_to_plugins("librime-lua" ${LIBRIME_LUA_SOURCE_DIR})

download_and_extract(
    "librime-predict"
    "master"
    ".zip"
    "https://github.com/rime/librime-predict/archive/refs/heads/master.zip"
    "bypass"
)
add_to_plugins("librime-predict" ${LIBRIME_PREDICT_SOURCE_DIR})
