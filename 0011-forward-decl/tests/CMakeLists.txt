cmake_minimum_required(VERSION 3.14)
project(0011_forward_decl_tests)

add_executable(test_forward_decl test_forward_decl.cpp)
target_include_directories(test_forward_decl PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_forward_decl PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_forward_decl COMMAND test_forward_decl)
