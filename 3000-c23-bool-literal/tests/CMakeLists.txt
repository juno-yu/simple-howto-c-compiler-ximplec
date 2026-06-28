cmake_minimum_required(VERSION 3.14)
project(c23_bool_literal_tests)

add_executable(test_c23_bool_literal test_c23_bool_literal.cpp)
target_include_directories(test_c23_bool_literal PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c23_bool_literal PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c23_bool_literal COMMAND test_c23_bool_literal)
