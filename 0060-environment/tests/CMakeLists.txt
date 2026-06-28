cmake_minimum_required(VERSION 3.14)
project(0060_environment_tests)

add_executable(test_environment test_environment.cpp)
target_include_directories(test_environment PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_environment PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_environment COMMAND test_environment)
