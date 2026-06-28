cmake_minimum_required(VERSION 3.14)
project(0032_break_continue_tests)

add_executable(test_break_continue test_break_continue.cpp)
target_include_directories(test_break_continue PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_break_continue PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_break_continue COMMAND test_break_continue)
