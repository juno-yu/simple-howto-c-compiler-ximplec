cmake_minimum_required(VERSION 3.14)
project(do_while_tests)

add_executable(test_do_while test_do_while.cpp)
target_include_directories(test_do_while PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_do_while PRIVATE compiler_lib Catch2::Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_do_while COMMAND test_do_while)
set_tests_properties(test_do_while PROPERTIES WILL_FAIL TRUE)
