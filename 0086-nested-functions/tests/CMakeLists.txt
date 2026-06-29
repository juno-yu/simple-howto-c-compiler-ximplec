cmake_minimum_required(VERSION 3.14)
project(0086_nested_functions_tests)

add_executable(test_nested test_nested.cpp)
target_include_directories(test_nested PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_nested PRIVATE compiler_lib Catch2::Catch2WithMain)

add_test(NAME test_nested COMMAND test_nested)
