cmake_minimum_required(VERSION 3.18)
project(kstd-core LANGUAGES C CXX)

option(KSTD_CORE_BUILD_TESTS "Build unit tests for kstd-core" OFF)
set(KSTD_CORE_MODULE ON)
add_compile_definitions(KSTD_CORE_MODULE)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;")
include(cmx)

if (${KSTD_CORE_BUILD_TESTS})
    cmx_include_scripts()
    cmx_add_tests(kstd-core-tests "${CMAKE_SOURCE_DIR}/test")
    target_include_directories(kstd-core-tests PRIVATE "${CMAKE_SOURCE_DIR}/include")
    cmx_include_fmt(kstd-core-tests PUBLIC)
endif ()
