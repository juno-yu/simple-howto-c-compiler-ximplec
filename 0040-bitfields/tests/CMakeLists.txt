cmake_minimum_required(VERSION 3.14)
project(0040_bitfields_tests)

add_executable(test_bitfields test_bitfields.cpp)
target_include_directories(test_bitfields PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_bitfields PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_bitfields COMMAND test_bitfields)
