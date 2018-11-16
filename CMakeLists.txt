cmake_minimum_required(VERSION 3.1)

project(baseline)

set (CMAKE_CXX_STANDARD 11)

option(THREAD_SUPPORT "Build with thread support" OFF)
option(BUILD_TESTS "Build unit tests" ON)

set(Baseline_VERSION_MAJOR 0)
set(Baseline_VERSION_MINOR 1)

find_package(Threads)

if(THREAD_SUPPORT AND Threads_FOUND)
  set(BASELINE_THREAD_SUPPORT ON)
else()
  set(BASELINE_THREAD_SUPPORT OFF)
endif()

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Baseline.h.in"
  "${PROJECT_BINARY_DIR}/include/baseline/Baseline.h"
  )

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Private.h.in"
  "${PROJECT_BINARY_DIR}/include/Private.h"
  )

include_directories(src include ${PROJECT_BINARY_DIR}/include)

file(GLOB Baseline_SRCS src/*.cpp)

add_library(baseline ${Baseline_SRCS})

install(TARGETS baseline DESTINATION lib)
install(DIRECTORY include/baseline DESTINATION include)
install(FILES ${PROJECT_BINARY_DIR}/include/baseline/Baseline.h DESTINATION include/baseline)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()