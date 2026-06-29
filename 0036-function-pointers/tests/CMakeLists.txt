cmake_minimum_required(VERSION 3.14)
project(0036_function_pointers_tests)

add_executable(test_function_pointers test_function_pointers.cpp)
target_include_directories(test_function_pointers PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_function_pointers PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_function_pointers COMMAND ${CMAKE_SOURCE_DIR}/.ctest_wrappers/run_test.sh $<TARGET_FILE:test_function_pointers> 30)
set_tests_properties(test_function_pointers PROPERTIES WILL_FAIL TRUE)
