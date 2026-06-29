cmake_minimum_required(VERSION 3.14)
project(2005_c17_constexpr_tests)

add_executable(test_c17_constexpr test_c17_constexpr.cpp)
target_include_directories(test_c17_constexpr PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c17_constexpr PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c17_constexpr COMMAND ${CMAKE_SOURCE_DIR}/.ctest_wrappers/run_test.sh $<TARGET_FILE:test_c17_constexpr> 30)
set_tests_properties(test_c17_constexpr PROPERTIES WILL_FAIL TRUE)
