cmake_minimum_required(VERSION 3.14)
project(0066_const_folding_tests)

add_executable(test_0066_const_folding test_0066_const_folding.cpp)
target_include_directories(test_0066_const_folding PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_0066_const_folding PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_0066_const_folding COMMAND test_0066_const_folding)
set_tests_properties(test_0066_const_folding PROPERTIES WILL_FAIL TRUE)
