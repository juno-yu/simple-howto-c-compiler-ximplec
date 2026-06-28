cmake_minimum_required(VERSION 3.14)
project(0037_void_pointers_tests)

add_executable(test_void_pointers test_void_pointers.cpp)
target_include_directories(test_void_pointers PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_void_pointers PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_void_pointers COMMAND test_void_pointers)
