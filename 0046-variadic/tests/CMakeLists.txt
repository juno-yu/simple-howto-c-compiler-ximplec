cmake_minimum_required(VERSION 3.14)
project(variadic_tests)

add_executable(test_variadic test_variadic.cpp)
target_include_directories(test_variadic PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_variadic PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_variadic COMMAND test_variadic)
