cmake_minimum_required(VERSION 3.14)
project(c23_auto_array_tests)

add_executable(test_c23_auto_array test_c23_auto_array.cpp)
target_include_directories(test_c23_auto_array PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c23_auto_array PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c23_auto_array COMMAND test_c23_auto_array)
set_tests_properties(test_c23_auto_array PROPERTIES WILL_FAIL TRUE)