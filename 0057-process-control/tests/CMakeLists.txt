cmake_minimum_required(VERSION 3.14)
project(0057_process_control_tests)

add_executable(test_process_control test_process_control.cpp)
target_include_directories(test_process_control PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_process_control PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_process_control COMMAND test_process_control)
