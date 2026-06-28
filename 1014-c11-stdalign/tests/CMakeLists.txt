cmake_minimum_required(VERSION 3.14)
project(1014_c11_stdalign_tests)

add_executable(test_stdalign test_stdalign.cpp)
target_include_directories(test_stdalign PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_stdalign PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_stdalign COMMAND test_stdalign)
