cmake_minimum_required(VERSION 3.14)
project(0027_unions_tests)

add_executable(test_unions test_unions.cpp)
target_include_directories(test_unions PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_unions PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_unions COMMAND test_unions)
set_tests_properties(test_unions PROPERTIES WILL_FAIL TRUE)
