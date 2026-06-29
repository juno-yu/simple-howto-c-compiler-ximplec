cmake_minimum_required(VERSION 3.14)
project(c23_for_init_decl_tests)

add_executable(test_c23_for_init_decl test_c23_for_init_decl.cpp)
target_include_directories(test_c23_for_init_decl PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c23_for_init_decl PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c23_for_init_decl COMMAND ${CMAKE_SOURCE_DIR}/.ctest_wrappers/run_test.sh $<TARGET_FILE:test_c23_for_init_decl> 30)
set_tests_properties(test_c23_for_init_decl PROPERTIES WILL_FAIL TRUE)
