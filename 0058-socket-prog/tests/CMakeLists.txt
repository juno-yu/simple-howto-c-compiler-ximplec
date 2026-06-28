cmake_minimum_required(VERSION 3.14)
project(0058_socket_prog_tests)

add_executable(test_socket_prog test_socket_prog.cpp)
target_include_directories(test_socket_prog PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_socket_prog PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_socket_prog COMMAND test_socket_prog)
