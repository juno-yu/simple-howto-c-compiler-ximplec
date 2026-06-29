cmake_minimum_required(VERSION 3.14)
project(0014_sizeof_tests)

add_executable(test_sizeof test_sizeof.cpp)
target_include_directories(test_sizeof PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_sizeof PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_sizeof COMMAND test_sizeof)
set_tests_properties(test_sizeof PROPERTIES WILL_FAIL TRUE)
