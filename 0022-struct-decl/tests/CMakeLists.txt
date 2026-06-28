cmake_minimum_required(VERSION 3.14)
project(0022_struct_decl_tests)

add_executable(test_struct_decl test_struct_decl.cpp)
target_include_directories(test_struct_decl PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_struct_decl PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_struct_decl COMMAND test_struct_decl)
