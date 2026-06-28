cmake_minimum_required(VERSION 3.14)
project(0043_float_double_tests)

add_executable(test_float_double test_float_double.cpp)
target_include_directories(test_float_double PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_float_double PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_float_double COMMAND test_float_double)
