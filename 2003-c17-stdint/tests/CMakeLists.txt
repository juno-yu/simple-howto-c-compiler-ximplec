cmake_minimum_required(VERSION 3.14)
project(2003_c17_stdint_tests)

add_executable(test_c17_stdint test_c17_stdint.cpp)
target_include_directories(test_c17_stdint PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c17_stdint PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c17_stdint COMMAND test_c17_stdint)
