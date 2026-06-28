cmake_minimum_required(VERSION 3.14)
project(0071_self_host_prep_tests)

add_executable(test_0071_self_host_prep test_0071_self_host_prep.cpp)
target_include_directories(test_0071_self_host_prep PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_0071_self_host_prep PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_0071_self_host_prep COMMAND test_0071_self_host_prep)
