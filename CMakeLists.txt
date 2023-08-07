cmake_minimum_required(VERSION 3.18)
project(kstd-core LANGUAGES C CXX)

option(KSTD_CORE_BUILD_TESTS "Build unit tests for kstd-core" OFF)
add_compile_definitions(KSTD_CORE_MODULE)

if(${KSTD_CORE_BUILD_TESTS})
    set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;")
    include(Cmx)
	cmx_include_scripts()
    cmx_add_tests(kstd-core-tests "${CMAKE_SOURCE_DIR}/test")
    target_include_directories(kstd-core-tests PRIVATE "${CMAKE_SOURCE_DIR}/include")
endif()
