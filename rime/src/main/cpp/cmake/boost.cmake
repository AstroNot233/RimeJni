set(BOOST_VERSION "1.89.0")
set(BOOST_URL "https://github.com/boostorg/boost/releases/download/boost-${BOOST_VERSION}/boost_${BOOST_VERSION}.tar.xz")

download_and_extract(
    "Boost"
    ${BOOST_VERSION}
    ".tar.xz"
    ${BOOST_URL}
    "875cc413afa6b86922b6df3b2ad23dec4511c8a741753e57c1129e7fa753d700"
)

include(ExternalProject)
add_custom_target(
    boost_build
    COMMAND ${BOOST_ROOT}/bootstrap.sh
    COMMAND ${BOOST_ROOT}/b2 toolset=clang target-os=android --prefix=${CMAKE_BINARY_DIR}/boost_install install
    WORKING_DIRECTORY ${BOOST_ROOT}
)

message(STATUS "Successfully built boost-${BOOST_VERSION}:")
message(STATUS ">>  ${BOOST_ROOT}")

set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_USE_STATIC_LIBS ON)

set(B2_WITH_ARGS "")
foreach(comp system filesystem interprocess signals2 uuid crc regex)
    list(APPEND B2_WITH_ARGS "--with-${comp}")
endforeach()

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/boost_install/include/boost/version.hpp
    COMMAND ${BOOST_ROOT}/bootstrap.sh
    COMMAND ${BOOST_ROOT}/b2
        toolset=clang
        target-os=android
        variant=release
        link=static
        runtime-link=static
        threading=multi
        --prefix=${CMAKE_BINARY_DIR}/boost_install
        ${B2_WITH_ARGS}
        install
    WORKING_DIRECTORY ${BOOST_ROOT}
    COMMENT "Building Boost for Android"
)

add_library(Boost::headers INTERFACE IMPORTED GLOBAL)
set_target_properties(Boost::headers PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_BINARY_DIR}/boost_install/include"
    INTERFACE_COMPILE_DEFINITIONS "BOOST_DLL_USE_STD_FS"
)

foreach(lib ${BOOST_LIBS})
    if(EXISTS "${CMAKE_BINARY_DIR}/boost_install/lib/libboost_${lib}.a")
        add_library(Boost::${lib} STATIC IMPORTED GLOBAL)
        set_target_properties(Boost::${lib} PROPERTIES
            IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/boost_install/lib/libboost_${lib}.a"
            INTERFACE_LINK_LIBRARIES "Boost::headers"
        )
        message(STATUS "Boost::${lib} target created")
    else()
        message(WARNING "Boost library ${lib} not built: libboost_${lib}.a not found")
    endif()
endforeach()