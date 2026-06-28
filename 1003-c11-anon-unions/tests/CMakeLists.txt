cmake_minimum_required(VERSION 3.14)
project(1003_c11_anon_unions_tests)

add_executable(test_c11_anon_unions test_c11_anon_unions.cpp)
target_include_directories(test_c11_anon_unions PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c11_anon_unions PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c11_anon_unions COMMAND test_c11_anon_unions)
