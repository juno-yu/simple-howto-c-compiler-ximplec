cmake_minimum_required(VERSION 3.14)
project(0031_goto_labels_tests)

add_executable(test_goto_labels test_goto_labels.cpp)
target_include_directories(test_goto_labels PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_goto_labels PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_goto_labels COMMAND test_goto_labels)
