cmake_minimum_required(VERSION 3.14)
project(2004_c17_typeof_tests)

add_executable(test_c17_typeof test_c17_typeof.cpp)
target_include_directories(test_c17_typeof PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c17_typeof PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c17_typeof COMMAND test_c17_typeof)
