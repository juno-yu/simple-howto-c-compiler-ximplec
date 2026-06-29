cmake_minimum_required(VERSION 3.14)
project(0061_thread_support_tests)

add_executable(test_thread_support test_thread_support.cpp)
target_include_directories(test_thread_support PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_thread_support PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_thread_support COMMAND ${CMAKE_SOURCE_DIR}/.ctest_wrappers/run_test.sh $<TARGET_FILE:test_thread_support> 30)
set_tests_properties(test_thread_support PROPERTIES WILL_FAIL TRUE)
