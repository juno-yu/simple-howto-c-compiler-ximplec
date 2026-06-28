cmake_minimum_required(VERSION 3.14)
project(2002_c17_stdnoreturn_tests)

add_executable(test_c17_stdnoreturn test_c17_stdnoreturn.cpp)
target_include_directories(test_c17_stdnoreturn PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c17_stdnoreturn PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c17_stdnoreturn COMMAND test_c17_stdnoreturn)
