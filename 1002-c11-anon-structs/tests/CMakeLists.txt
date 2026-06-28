cmake_minimum_required(VERSION 3.14)
project(1002_c11_anon_structs_tests)

add_executable(test_c11_anon_structs test_c11_anon_structs.cpp)
target_include_directories(test_c11_anon_structs PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c11_anon_structs PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c11_anon_structs COMMAND test_c11_anon_structs)
