cmake_minimum_required(VERSION 3.14)
project(0070_debug_info_tests)

add_executable(test_0070_debug_info test_0070_debug_info.cpp)
target_include_directories(test_0070_debug_info PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_0070_debug_info PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_0070_debug_info COMMAND test_0070_debug_info)
