cmake_minimum_required(VERSION 3.14)
project(0021_extern_declarations_tests)

add_executable(test_extern_declarations test_extern_declarations.cpp)
target_include_directories(test_extern_declarations PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_extern_declarations PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_extern_declarations COMMAND test_extern_declarations)
