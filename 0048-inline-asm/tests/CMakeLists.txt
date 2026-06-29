cmake_minimum_required(VERSION 3.14)
project(inline_asm_tests)

add_executable(test_inline_asm test_inline_asm.cpp)
target_include_directories(test_inline_asm PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_inline_asm PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_inline_asm COMMAND ${CMAKE_SOURCE_DIR}/.ctest_wrappers/run_test.sh $<TARGET_FILE:test_inline_asm> 30)
set_tests_properties(test_inline_asm PROPERTIES WILL_FAIL TRUE)
