cmake_minimum_required(VERSION 3.14)
project(1008_c11_tgmath_tests)

add_executable(test_tgmath test_tgmath.cpp)
target_include_directories(test_tgmath PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_tgmath PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_tgmath COMMAND test_tgmath)
