cmake_minimum_required(VERSION 3.14)
project(c23_auto_type_tests)

add_executable(test_c23_auto_type test_c23_auto_type.cpp)
target_include_directories(test_c23_auto_type PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c23_auto_type PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c23_auto_type COMMAND test_c23_auto_type)
set_tests_properties(test_c23_auto_type PROPERTIES WILL_FAIL TRUE)
