cmake_minimum_required(VERSION 3.14)
project(0069_function_inline_tests)

add_executable(test_0069_function_inline test_0069_function_inline.cpp)
target_include_directories(test_0069_function_inline PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_0069_function_inline PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_0069_function_inline COMMAND test_0069_function_inline)
