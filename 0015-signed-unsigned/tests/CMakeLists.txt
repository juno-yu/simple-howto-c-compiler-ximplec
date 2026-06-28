cmake_minimum_required(VERSION 3.14)
project(0015_signed_unsigned_tests)

add_executable(test_signed_unsigned test_signed_unsigned.cpp)
target_include_directories(test_signed_unsigned PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_signed_unsigned PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_signed_unsigned COMMAND test_signed_unsigned)
