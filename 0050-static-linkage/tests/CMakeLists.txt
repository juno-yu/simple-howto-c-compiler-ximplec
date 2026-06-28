cmake_minimum_required(VERSION 3.14)
project(static_linkage_tests)

add_executable(test_static_linkage test_static_linkage.cpp)
target_include_directories(test_static_linkage PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_static_linkage PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_static_linkage COMMAND test_static_linkage)
