cmake_minimum_required(VERSION 3.14)
project(0056_file_io_tests)

add_executable(test_file_io test_file_io.cpp)
target_include_directories(test_file_io PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_file_io PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_file_io COMMAND test_file_io)
