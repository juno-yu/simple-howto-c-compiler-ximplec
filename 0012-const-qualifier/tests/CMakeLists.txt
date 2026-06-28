cmake_minimum_required(VERSION 3.14)
project(0012_const_qualifier_tests)

add_executable(test_const_qualifier test_const_qualifier.cpp)
target_include_directories(test_const_qualifier PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_const_qualifier PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_const_qualifier COMMAND test_const_qualifier)
