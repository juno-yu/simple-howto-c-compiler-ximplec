cmake_minimum_required(VERSION 3.14)
project(1006_c11_threads_tests)

add_executable(test_c11_threads test_c11_threads.cpp)
target_include_directories(test_c11_threads PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c11_threads PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c11_threads COMMAND test_c11_threads)
