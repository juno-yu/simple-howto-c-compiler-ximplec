cmake_minimum_required(VERSION 3.14)
project(statement_expr_tests)

add_executable(test_statement_expr test_statement_expr.cpp)
target_include_directories(test_statement_expr PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_statement_expr PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_statement_expr COMMAND test_statement_expr)
