cmake_minimum_required(VERSION 3.14)
project(1004_c11_noreturn_tests)

add_executable(test_c11_noreturn test_c11_noreturn.cpp)
target_include_directories(test_c11_noreturn PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c11_noreturn PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c11_noreturn COMMAND test_c11_noreturn)
