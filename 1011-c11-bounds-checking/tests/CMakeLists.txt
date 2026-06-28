cmake_minimum_required(VERSION 3.14)
project(1011_c11_bounds_checking_tests)

add_executable(test_bounds_checking test_bounds_checking.cpp)
target_include_directories(test_bounds_checking PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_bounds_checking PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_bounds_checking COMMAND test_bounds_checking)
