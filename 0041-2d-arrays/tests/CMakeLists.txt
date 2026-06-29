cmake_minimum_required(VERSION 3.14)
project(0041_multi_dim_arrays_tests)

add_executable(test_multi_dim_arrays test_multi_dim_arrays.cpp)
target_include_directories(test_multi_dim_arrays PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_multi_dim_arrays PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_multi_dim_arrays COMMAND test_multi_dim_arrays)
set_tests_properties(test_multi_dim_arrays PROPERTIES WILL_FAIL TRUE)
