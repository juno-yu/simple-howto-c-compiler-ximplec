cmake_minimum_required(VERSION 3.14)
project(0024_pointer_types_tests)

add_executable(test_pointer_types test_pointer_types.cpp)
target_include_directories(test_pointer_types PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_pointer_types PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_pointer_types COMMAND test_pointer_types)
