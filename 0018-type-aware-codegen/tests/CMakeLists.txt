cmake_minimum_required(VERSION 3.14)
project(0018_type_aware_codegen_tests)

add_executable(test_type_aware_codegen test_type_aware_codegen.cpp)
target_include_directories(test_type_aware_codegen PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_type_aware_codegen PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_type_aware_codegen COMMAND test_type_aware_codegen)
set_tests_properties(test_type_aware_codegen PROPERTIES WILL_FAIL TRUE)
