cmake_minimum_required(VERSION 3.18)
project("kstd-core" LANGUAGES C CXX)

option(KSTD_CORE_BUILD_TESTS "Build unit tests for kstd-core" ON)
option(KSTD_CORE_EMBEDDED "Look up CMake infra in parent directory instead of the project directory" OFF)
set(KSTD_CORE_MODULE ON) # Let other modules know we are present!
add_compile_definitions(KSTD_CORE_MODULE)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    message(STATUS "No C++ version specified, defaulting to C++20")
    set(CMAKE_CXX_STANDARD 20)
endif()

if(${KSTD_CORE_EMBEDDED})
    set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../cmake;")
else()
    set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;")
endif()

include(CommonLibraries)
include(Platform)

if(${KSTD_CORE_BUILD_TESTS})
    file(GLOB_RECURSE KSTD_SOURCE_FILES "${CMAKE_SOURCE_DIR}/test/*.cpp")
    add_executable("${CMAKE_PROJECT_NAME}" "${KSTD_SOURCE_FILES}")
    target_include_fmt("${CMAKE_PROJECT_NAME}")
    target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_SOURCE_DIR}/include")
    target_include_gtest("${CMAKE_PROJECT_NAME}")
endif()
