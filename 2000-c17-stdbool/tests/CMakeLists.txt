cmake_minimum_required(VERSION 3.14)
project(2000_c17_stdbool_tests)

add_executable(test_c17_stdbool test_c17_stdbool.cpp)
target_include_directories(test_c17_stdbool PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c17_stdbool PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c17_stdbool COMMAND test_c17_stdbool)
