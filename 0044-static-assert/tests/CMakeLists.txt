cmake_minimum_required(VERSION 3.14)
project(0044_static_assert_tests)

add_executable(test_static_assert test_static_assert.cpp)
target_include_directories(test_static_assert PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_static_assert PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_static_assert COMMAND test_static_assert)
