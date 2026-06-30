cmake_minimum_required(VERSION 3.14)
project(1000_c11_static_assert_tests)

add_executable(test_c11_static_assert test_c11_static_assert.cpp)
target_include_directories(test_c11_static_assert PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c11_static_assert PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c11_static_assert COMMAND test_c11_static_assert)
