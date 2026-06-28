cmake_minimum_required(VERSION 3.14)
project(0051_volatile_qualifier_tests)

add_executable(test_volatile_qualifier test_volatile_qualifier.cpp)
target_include_directories(test_volatile_qualifier PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_volatile_qualifier PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_volatile_qualifier COMMAND test_volatile_qualifier)
