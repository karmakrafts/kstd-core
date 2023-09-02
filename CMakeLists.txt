cmake_minimum_required(VERSION 3.18)
project(kstd-core LANGUAGES C CXX)

option(KSTD_CORE_BUILD_TESTS "Build unit tests for kstd-core" OFF)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;")
include(cmx-bootstrap)

cmx_include_scripts()
cmx_add_library(kstd-core INTERFACE)
target_include_directories(kstd-core INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
cmx_include_fmt(kstd-core INTERFACE)

if (${KSTD_CORE_BUILD_TESTS})
    cmx_add_tests(kstd-core-tests "${CMAKE_CURRENT_SOURCE_DIR}/test")
    target_link_libraries(kstd-core-tests PRIVATE kstd-core)
    add_dependencies(kstd-core-tests kstd-core)
endif ()
