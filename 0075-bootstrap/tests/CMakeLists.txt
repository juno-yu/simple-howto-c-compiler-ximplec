cmake_minimum_required(VERSION 3.14)
project(0075_bootstrap_tests)

add_executable(test_0075_bootstrap test_0075_bootstrap.cpp)
target_include_directories(test_0075_bootstrap PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_0075_bootstrap PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_0075_bootstrap COMMAND test_0075_bootstrap)
