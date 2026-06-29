cmake_minimum_required(VERSION 3.14)
project(1007_c11_memalign_tests)

add_executable(test_c11_memalign test_c11_memalign.cpp)
target_include_directories(test_c11_memalign PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_c11_memalign PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_c11_memalign COMMAND ${CMAKE_SOURCE_DIR}/.ctest_wrappers/run_test.sh $<TARGET_FILE:test_c11_memalign> 30)
set_tests_properties(test_c11_memalign PROPERTIES WILL_FAIL TRUE)
