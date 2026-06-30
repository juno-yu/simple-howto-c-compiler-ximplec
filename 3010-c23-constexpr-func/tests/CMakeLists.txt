cmake_minimum_required(VERSION 3.14)
project(c23_constexpr_func_tests)

add_executable(test_c23_constexpr_func test_c23_constexpr_func.cpp)
target_include_directories(test_c23_constexpr_func PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c23_constexpr_func PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c23_constexpr_func COMMAND test_c23_constexpr_func)
