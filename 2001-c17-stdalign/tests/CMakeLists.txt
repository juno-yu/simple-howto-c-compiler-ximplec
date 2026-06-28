cmake_minimum_required(VERSION 3.14)
project(2001_c17_stdalign_tests)

add_executable(test_c17_stdalign test_c17_stdalign.cpp)
target_include_directories(test_c17_stdalign PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c17_stdalign PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c17_stdalign COMMAND test_c17_stdalign)
