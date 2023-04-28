cmake_minimum_required(VERSION 3.18)
project(kstd-types LANGUAGES C CXX)

option(KSTD_TYPES_BUILD_TESTS "Build unit tests for kstd-types" ON)
set(KSTD_TYPES_MODULE ON) # Let other modules know we are present!
add_compile_definitions(KSTD_TYPES_MODULE)

set(CMAKE_CXX_STANDARD 20)

if(${KSTD_TYPES_EMBEDDED})
    set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../cmake;")
else()
    set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;")
endif()

include(CommonLibraries)
include(Platform)

if(${KSTD_TYPES_BUILD_TESTS})
    file(GLOB_RECURSE KSTD_SOURCE_FILES "${CMAKE_SOURCE_DIR}/test/*.cpp")
    add_executable(${CMAKE_PROJECT_NAME} ${KSTD_SOURCE_FILES})
    target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include")
    target_include_gtest(${CMAKE_PROJECT_NAME})
endif()