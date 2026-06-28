cmake_minimum_required(VERSION 3.14)
project(1015_c11_stdatomic_complete_tests)

add_executable(test_stdatomic_complete test_stdatomic_complete.cpp)
target_include_directories(test_stdatomic_complete PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_stdatomic_complete PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_stdatomic_complete COMMAND test_stdatomic_complete)
