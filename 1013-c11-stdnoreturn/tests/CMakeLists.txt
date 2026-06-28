cmake_minimum_required(VERSION 3.14)
project(1013_c11_stdnoreturn_tests)

add_executable(test_stdnoreturn test_stdnoreturn.cpp)
target_include_directories(test_stdnoreturn PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_stdnoreturn PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_stdnoreturn COMMAND test_stdnoreturn)
