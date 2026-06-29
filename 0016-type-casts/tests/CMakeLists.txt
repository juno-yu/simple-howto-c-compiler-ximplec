cmake_minimum_required(VERSION 3.14)
project(0016_type_casts_tests)

add_executable(test_type_casts test_type_casts.cpp)
target_include_directories(test_type_casts PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_type_casts PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_type_casts COMMAND test_type_casts)
set_tests_properties(test_type_casts PROPERTIES WILL_FAIL TRUE)
