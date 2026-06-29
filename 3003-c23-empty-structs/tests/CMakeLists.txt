cmake_minimum_required(VERSION 3.14)
project(c23_empty_structs_tests)

add_executable(test_c23_empty_structs test_c23_empty_structs.cpp)
target_include_directories(test_c23_empty_structs PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c23_empty_structs PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c23_empty_structs COMMAND test_c23_empty_structs)
set_tests_properties(test_c23_empty_structs PROPERTIES WILL_FAIL TRUE)
