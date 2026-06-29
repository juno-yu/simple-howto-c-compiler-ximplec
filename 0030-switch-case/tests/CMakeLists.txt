cmake_minimum_required(VERSION 3.14)
project(0030_switch_case_tests)

add_executable(test_switch_case test_switch_case.cpp)
target_include_directories(test_switch_case PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_switch_case PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_switch_case COMMAND test_switch_case)
set_tests_properties(test_switch_case PROPERTIES WILL_FAIL TRUE)
