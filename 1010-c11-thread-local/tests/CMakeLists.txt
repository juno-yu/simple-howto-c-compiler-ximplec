cmake_minimum_required(VERSION 3.14)
project(1010_c11_thread_local_tests)

add_executable(test_thread_local test_thread_local.cpp)
target_include_directories(test_thread_local PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_thread_local PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_thread_local COMMAND test_thread_local)
