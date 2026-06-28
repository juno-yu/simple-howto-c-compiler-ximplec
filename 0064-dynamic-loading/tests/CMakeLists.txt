cmake_minimum_required(VERSION 3.14)
project(0064_dynamic_loading_tests)

add_executable(test_dynamic_loading test_dynamic_loading.cpp)
target_include_directories(test_dynamic_loading PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_dynamic_loading PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_dynamic_loading COMMAND test_dynamic_loading)
