cmake_minimum_required(VERSION 3.18)
project(kstd-core LANGUAGES C CXX)

option(KSTD_CORE_BUILD_TESTS "Build unit tests for kstd-core" ON)
add_compile_definitions(KSTD_CORE_MODULE)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    message(STATUS "No C++ version specified, defaulting to C++20")
    set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;")
include(Platform)
include(CommonLibraries)
include_scripts()

if(${KSTD_CORE_BUILD_TESTS})
    file(GLOB_RECURSE KSTD_CORE_TEST_SOURCE_FILES "${CMAKE_SOURCE_DIR}/test/*.cpp")
    add_executable(kstd-core-tests "${KSTD_CORE_TEST_SOURCE_FILES}")
    target_include_directories(kstd-core-tests PUBLIC "${CMAKE_SOURCE_DIR}/include")
    target_include_gtest(kstd-core-tests)
endif()
