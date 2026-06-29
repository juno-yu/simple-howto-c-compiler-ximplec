cmake_minimum_required(VERSION 3.14)
project(codegen_coverage_tests)

add_executable(test_codegen_coverage test_codegen_coverage.cpp)
target_include_directories(test_codegen_coverage PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_codegen_coverage PRIVATE compiler_lib Catch2::Catch2WithMain)

add_test(NAME codegen_coverage_tests COMMAND test_codegen_coverage)
