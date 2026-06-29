cmake_minimum_required(VERSION 3.14)
project(0042_array_pointer_decay_tests)

add_executable(test_array_pointer_decay test_array_pointer_decay.cpp)
target_include_directories(test_array_pointer_decay PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_array_pointer_decay PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_array_pointer_decay COMMAND test_array_pointer_decay)
set_tests_properties(test_array_pointer_decay PROPERTIES WILL_FAIL TRUE)
