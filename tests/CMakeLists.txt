
enable_testing()

add_executable(MathTests MathTests.cpp)
target_link_libraries(MathTests baseline)
add_test(MathTests MathTests)

add_executable(IOTests IOTests.cpp)
target_link_libraries(IOTests baseline)
add_test(IOTests IOTests)