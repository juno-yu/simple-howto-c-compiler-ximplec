cmake_minimum_required(VERSION 3.14)
project(ternary_tests)

add_executable(test_ternary test_ternary.cpp)
target_include_directories(test_ternary PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_ternary PRIVATE compiler_lib Catch2::Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_ternary COMMAND ${CMAKE_SOURCE_DIR}/.ctest_wrappers/run_test.sh $<TARGET_FILE:test_ternary> 30)
set_tests_properties(test_ternary PROPERTIES WILL_FAIL TRUE)
