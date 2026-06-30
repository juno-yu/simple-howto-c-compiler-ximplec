cmake_minimum_required(VERSION 3.14)
project(0033_preprocessor_macros_tests)

add_executable(test_preprocessor_macros test_preprocessor_macros.cpp)
target_include_directories(test_preprocessor_macros PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_preprocessor_macros PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_preprocessor_macros COMMAND test_preprocessor_macros)
