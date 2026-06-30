cmake_minimum_required(VERSION 3.14)
project(9999_parser_bugfixes_tests)

add_executable(test_parser_bugfixes test_parser_bugfixes.cpp)
target_include_directories(test_parser_bugfixes PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_parser_bugfixes PRIVATE compiler_lib Catch2::Catch2WithMain)

add_test(NAME test_parser_bugfixes COMMAND test_parser_bugfixes)
