
enable_testing()

add_executable(MathTests MathTests.cpp)
target_link_libraries(MathTests baseline)
add_test(MathTests MathTests)