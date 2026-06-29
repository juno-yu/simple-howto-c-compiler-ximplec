cmake_minimum_required(VERSION 3.14)
project(bool_type_tests)

add_executable(test_bool_type test_bool_type.cpp)
target_include_directories(test_bool_type PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_bool_type PRIVATE compiler_lib Catch2::Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_bool_type COMMAND test_bool_type)
set_tests_properties(test_bool_type PROPERTIES WILL_FAIL TRUE)
