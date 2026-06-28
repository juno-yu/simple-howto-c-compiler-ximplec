cmake_minimum_required(VERSION 3.14)
project(0045_generic_tests)

add_executable(test_generic test_generic.cpp)
target_include_directories(test_generic PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_generic PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_generic COMMAND test_generic)
