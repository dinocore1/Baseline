
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