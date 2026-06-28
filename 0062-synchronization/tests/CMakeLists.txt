cmake_minimum_required(VERSION 3.14)
project(0062_synchronization_tests)

add_executable(test_synchronization test_synchronization.cpp)
target_include_directories(test_synchronization PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_synchronization PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_synchronization COMMAND test_synchronization)
