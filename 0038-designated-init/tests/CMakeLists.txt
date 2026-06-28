cmake_minimum_required(VERSION 3.14)
project(0038_designated_init_tests)

add_executable(test_designated_init test_designated_init.cpp)
target_include_directories(test_designated_init PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_designated_init PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_designated_init COMMAND test_designated_init)
