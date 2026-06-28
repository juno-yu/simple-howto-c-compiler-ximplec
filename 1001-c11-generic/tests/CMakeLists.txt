cmake_minimum_required(VERSION 3.14)
project(1001_c11_generic_tests)

add_executable(test_c11_generic test_c11_generic.cpp)
target_include_directories(test_c11_generic PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c11_generic PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c11_generic COMMAND test_c11_generic)
