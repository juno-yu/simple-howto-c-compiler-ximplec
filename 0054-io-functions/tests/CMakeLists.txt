cmake_minimum_required(VERSION 3.14)
project(0054_io_functions_tests)

add_executable(test_io_functions test_io_functions.cpp)
target_include_directories(test_io_functions PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_io_functions PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_io_functions COMMAND test_io_functions)
