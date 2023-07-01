cmake_minimum_required(VERSION 3.18)
project("kstd-core" LANGUAGES C CXX)

option(KSTD_CORE_BUILD_TESTS "Build unit tests for kstd-core" ON)
set(KSTD_CORE_MODULE ON) # Let other modules know we are present!
add_compile_definitions(KSTD_CORE_MODULE)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    message(STATUS "No C++ version specified, defaulting to C++20")
    set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;")
include(CommonLibraries)
include(Platform)

macro(target_include_kstd_core target)
    target_include_directories(${target} PUBLIC "${CMAKE_SOURCE_DIR}/include")
    target_include_fmt(${target})
endmacro()

if(${KSTD_CORE_BUILD_TESTS})
    file(GLOB_RECURSE KSTD_TEST_SOURCE_FILES "${CMAKE_SOURCE_DIR}/test/*.cpp")
    add_executable("${CMAKE_PROJECT_NAME}_tests" "${KSTD_TEST_SOURCE_FILES}")
    target_include_kstd_core("${CMAKE_PROJECT_NAME}_tests")
    target_include_gtest("${CMAKE_PROJECT_NAME}_tests")
endif()
