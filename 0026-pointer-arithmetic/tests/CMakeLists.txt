cmake_minimum_required(VERSION 3.14)
project(0026_pointer_arithmetic_tests)

add_executable(test_pointer_arithmetic test_pointer_arithmetic.cpp)
target_include_directories(test_pointer_arithmetic PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_pointer_arithmetic PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_pointer_arithmetic COMMAND test_pointer_arithmetic)
