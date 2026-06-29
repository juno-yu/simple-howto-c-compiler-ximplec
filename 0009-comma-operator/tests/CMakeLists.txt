cmake_minimum_required(VERSION 3.14)
project(comma_operator_tests)

add_executable(test_comma_operator test_comma_operator.cpp)
target_include_directories(test_comma_operator PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_comma_operator PRIVATE compiler_lib Catch2::Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_comma_operator COMMAND test_comma_operator)
set_tests_properties(test_comma_operator PROPERTIES WILL_FAIL TRUE)
