cmake_minimum_required(VERSION 3.14)
project(compound_assign_tests)

add_executable(test_compound_assign test_compound_assign.cpp)
target_include_directories(test_compound_assign PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_compound_assign PRIVATE compiler_lib Catch2::Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_compound_assign COMMAND test_compound_assign)
