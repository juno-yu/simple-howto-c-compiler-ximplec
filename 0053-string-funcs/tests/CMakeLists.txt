cmake_minimum_required(VERSION 3.14)
project(0053_string_funcs_tests)

add_executable(test_string_funcs test_string_funcs.cpp)
target_include_directories(test_string_funcs PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_string_funcs PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_string_funcs COMMAND test_string_funcs)
