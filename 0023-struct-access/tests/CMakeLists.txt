cmake_minimum_required(VERSION 3.14)
project(0023_struct_access_tests)

add_executable(test_struct_access test_struct_access.cpp)
target_include_directories(test_struct_access PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_struct_access PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_struct_access COMMAND test_struct_access)
