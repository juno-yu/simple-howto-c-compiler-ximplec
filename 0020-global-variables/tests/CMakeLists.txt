cmake_minimum_required(VERSION 3.14)
project(0020_global_variables_tests)

add_executable(test_global_variables test_global_variables.cpp)
target_include_directories(test_global_variables PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_global_variables PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_global_variables COMMAND test_global_variables)
