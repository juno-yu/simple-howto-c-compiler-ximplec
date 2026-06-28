cmake_minimum_required(VERSION 3.14)
project(0039_compound_literals_tests)

add_executable(test_compound_literals test_compound_literals.cpp)
target_include_directories(test_compound_literals PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_compound_literals PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_compound_literals COMMAND test_compound_literals)
