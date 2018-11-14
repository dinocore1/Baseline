cmake_minimum_required(VERSION 2.95)

project(baseline)

option(BUILD_TESTS "Build unit tests" ON)

set(Baseline_VERSION_MAJOR 0)
set(Baseline_VERSION_MINOR 1)

include_directories(src include)

file(GLOB Baseline_SRCS src/*.cpp)

add_library(baseline ${Baseline_SRCS})

install(TARGETS baseline DESTINATION lib)
install(DIRECTORY include DESTINATION include)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()