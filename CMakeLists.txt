cmake_minimum_required(VERSION 3.1)

project(baseline)

set (CMAKE_CXX_STANDARD 11)

option(THREAD_SUPPORT "Build with thread support" ON)
option(BUILD_TESTS "Build unit tests" ON)

set(Baseline_VERSION_MAJOR 0)
set(Baseline_VERSION_MINOR 1)
set(Baseline_VERSION_PATCH 1)

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

include_directories(src include ${PROJECT_BINARY_DIR}/include)

list(APPEND Baseline_SRCS
  src/Atomic.cpp
  src/Debug.cpp
  src/ExecutorService.cpp
  src/Hash.cpp
  src/Log.cpp
  src/MathUtils.cpp
  src/RefBase.cpp
  src/SharedBuffer.cpp
  src/Static.cpp
  src/Streams.cpp
  src/String8.cpp
  src/String16.cpp
  src/TextOutput.cpp
  src/Unicode.cpp
  src/VectorImpl.cpp
)

if(BASELINE_THREAD_SUPPORT)
  list(APPEND Baseline_SRCS
    src/Condition.cpp
    src/Mutex.cpp
	  src/Thread.cpp
  )
endif()


add_library(baseline ${Baseline_SRCS})

if(BASELINE_THREAD_SUPPORT)
  target_link_libraries(baseline Threads::Threads)
endif()

install(TARGETS baseline DESTINATION lib)
install(DIRECTORY include/baseline DESTINATION include)
install(FILES ${PROJECT_BINARY_DIR}/include/baseline/Baseline.h DESTINATION include/baseline)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()