cmake_minimum_required(VERSION 3.14)
project(c23_digit_separators_tests)

add_executable(test_c23_digit_separators test_c23_digit_separators.cpp)
target_include_directories(test_c23_digit_separators PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c23_digit_separators PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c23_digit_separators COMMAND test_c23_digit_separators)
