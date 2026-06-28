cmake_minimum_required(VERSION 3.14)
project(0067_dead_code_elim_tests)

add_executable(test_0067_dead_code_elim test_0067_dead_code_elim.cpp)
target_include_directories(test_0067_dead_code_elim PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_0067_dead_code_elim PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_0067_dead_code_elim COMMAND test_0067_dead_code_elim)
