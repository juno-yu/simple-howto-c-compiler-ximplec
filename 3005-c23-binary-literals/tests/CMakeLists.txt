cmake_minimum_required(VERSION 3.14)
project(c23_binary_literals_tests)

add_executable(test_c23_binary_literals test_c23_binary_literals.cpp)
target_include_directories(test_c23_binary_literals PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c23_binary_literals PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c23_binary_literals COMMAND test_c23_binary_literals)
set_tests_properties(test_c23_binary_literals PROPERTIES WILL_FAIL TRUE)
