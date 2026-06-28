cmake_minimum_required(VERSION 3.14)
project(c23_nullptr_tests)

add_executable(test_c23_nullptr test_c23_nullptr.cpp)
target_include_directories(test_c23_nullptr PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c23_nullptr PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c23_nullptr COMMAND test_c23_nullptr)