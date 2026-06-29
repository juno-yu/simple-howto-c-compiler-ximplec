cmake_minimum_required(VERSION 3.14)
project(0043_float_double_sse_tests)

add_executable(test_sse test_sse.cpp)
target_include_directories(test_sse PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_sse PRIVATE compiler_lib Catch2::Catch2WithMain)

add_test(NAME test_sse COMMAND test_sse)
