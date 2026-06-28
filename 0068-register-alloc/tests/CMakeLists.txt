cmake_minimum_required(VERSION 3.14)
project(0068_register_alloc_tests)

add_executable(test_0068_register_alloc test_0068_register_alloc.cpp)
target_include_directories(test_0068_register_alloc PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_0068_register_alloc PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_0068_register_alloc COMMAND test_0068_register_alloc)
