cmake_minimum_required(VERSION 3.14)
project(0065_ipc_tests)

add_executable(test_ipc test_ipc.cpp)
target_include_directories(test_ipc PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_ipc PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_ipc COMMAND test_ipc)
