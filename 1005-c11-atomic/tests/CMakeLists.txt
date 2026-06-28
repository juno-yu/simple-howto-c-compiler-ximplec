cmake_minimum_required(VERSION 3.14)
project(1005_c11_atomic_tests)

add_executable(test_c11_atomic test_c11_atomic.cpp)
target_include_directories(test_c11_atomic PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c11_atomic PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c11_atomic COMMAND test_c11_atomic)
