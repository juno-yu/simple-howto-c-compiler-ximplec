cmake_minimum_required(VERSION 3.14)
project(0052_syscalls_tests)

add_executable(test_syscalls test_syscalls.cpp)
target_include_directories(test_syscalls PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_syscalls PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_syscalls COMMAND test_syscalls)
