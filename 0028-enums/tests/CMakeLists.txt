cmake_minimum_required(VERSION 3.14)
project(0028_enums_tests)

add_executable(test_enums test_enums.cpp)
target_include_directories(test_enums PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_enums PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_enums COMMAND test_enums)
set_tests_properties(test_enums PROPERTIES WILL_FAIL TRUE)
