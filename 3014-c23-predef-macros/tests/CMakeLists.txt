cmake_minimum_required(VERSION 3.14)
project(c23_predef_macros_tests)

add_executable(test_c23_predef_macros test_c23_predef_macros.cpp)
target_include_directories(test_c23_predef_macros PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c23_predef_macros PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c23_predef_macros COMMAND ${CMAKE_SOURCE_DIR}/.ctest_wrappers/run_test.sh $<TARGET_FILE:test_c23_predef_macros> 30)