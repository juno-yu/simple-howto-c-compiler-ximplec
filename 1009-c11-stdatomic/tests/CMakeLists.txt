cmake_minimum_required(VERSION 3.14)
project(1009_c11_stdatomic_tests)

add_executable(test_stdatomic test_stdatomic.cpp)
target_include_directories(test_stdatomic PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_stdatomic PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_stdatomic COMMAND test_stdatomic)
