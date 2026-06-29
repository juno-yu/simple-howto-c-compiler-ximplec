cmake_minimum_required(VERSION 3.14)
project(c23_noreturn_attr_tests)

add_executable(test_c23_noreturn_attr test_c23_noreturn_attr.cpp)
target_include_directories(test_c23_noreturn_attr PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c23_noreturn_attr PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c23_noreturn_attr COMMAND test_c23_noreturn_attr)
set_tests_properties(test_c23_noreturn_attr PROPERTIES WILL_FAIL TRUE)