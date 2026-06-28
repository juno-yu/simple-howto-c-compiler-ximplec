cmake_minimum_required(VERSION 3.14)
project(0063_memory_mapping_tests)

add_executable(test_memory_mapping test_memory_mapping.cpp)
target_include_directories(test_memory_mapping PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_memory_mapping PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_memory_mapping COMMAND test_memory_mapping)
