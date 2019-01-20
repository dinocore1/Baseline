
enable_testing()

add_executable(MathTests MathTests.cpp)
target_link_libraries(MathTests baseline)
add_test(MathTests MathTests)

add_executable(IOTests IOTests.cpp)
target_link_libraries(IOTests baseline)
add_test(IOTests IOTests)

add_executable(VectorTests VectorTests.cpp)
target_link_libraries(VectorTests baseline)
add_test(VectorTests VectorTests)

add_executable(PointerTests PointerTests.cpp)
target_link_libraries(PointerTests baseline)
add_test(PointerTests PointerTests)

add_executable(TypeHelperTests TypeHelperTests.cpp)
target_link_libraries(TypeHelperTests baseline)
add_test(TypeHelperTests TypeHelperTests)

add_executable(LoggingTests LoggingTests.cpp)
target_link_libraries(LoggingTests baseline)
add_test(LoggingTests LoggingTests)

if(BASELINE_THREAD_SUPPORT)
  add_executable(ThreadTests ThreadTests.cpp)
  target_link_libraries(ThreadTests baseline)
  add_test(ThreadTests ThreadTests)

  add_executable(ExecutorServiceTests ExecutorServiceTests.cpp)
  target_link_libraries(ExecutorServiceTests baseline)
  add_test(ExecutorServiceTests ExecutorServiceTests)
endif()