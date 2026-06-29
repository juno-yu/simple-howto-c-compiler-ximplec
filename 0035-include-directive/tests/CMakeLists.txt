cmake_minimum_required(VERSION 3.14)
project(0035_include_directive_tests)

add_executable(test_include_directive test_include_directive.cpp)
target_include_directories(test_include_directive PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_include_directive PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_include_directive COMMAND test_include_directive)
set_tests_properties(test_include_directive PROPERTIES WILL_FAIL TRUE)
