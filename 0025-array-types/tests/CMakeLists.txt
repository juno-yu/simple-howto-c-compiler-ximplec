cmake_minimum_required(VERSION 3.14)
project(0025_array_types_tests)

add_executable(test_array_types test_array_types.cpp)
target_include_directories(test_array_types PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_array_types PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_array_types COMMAND test_array_types)
