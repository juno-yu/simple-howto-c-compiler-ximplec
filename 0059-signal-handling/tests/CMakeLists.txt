cmake_minimum_required(VERSION 3.14)
project(0059_signal_handling_tests)

add_executable(test_signal_handling test_signal_handling.cpp)
target_include_directories(test_signal_handling PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_signal_handling PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_signal_handling COMMAND ${CMAKE_SOURCE_DIR}/.ctest_wrappers/run_test.sh $<TARGET_FILE:test_signal_handling> 30)
set_tests_properties(test_signal_handling PROPERTIES WILL_FAIL TRUE)
