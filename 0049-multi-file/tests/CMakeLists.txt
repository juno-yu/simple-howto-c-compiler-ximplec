cmake_minimum_required(VERSION 3.14)
project(multi_file_tests)

add_executable(test_multi_file test_multi_file.cpp)
target_include_directories(test_multi_file PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_multi_file PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_multi_file COMMAND test_multi_file)
