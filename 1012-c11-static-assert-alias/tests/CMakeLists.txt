cmake_minimum_required(VERSION 3.14)
project(1012_c11_static_assert_alias_tests)

add_executable(test_static_assert_alias test_static_assert_alias.cpp)
target_include_directories(test_static_assert_alias PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_static_assert_alias PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_static_assert_alias COMMAND test_static_assert_alias)
