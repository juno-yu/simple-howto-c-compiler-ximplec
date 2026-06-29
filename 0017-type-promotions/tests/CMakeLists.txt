cmake_minimum_required(VERSION 3.14)
project(0017_type_promotions_tests)

add_executable(test_type_promotions test_type_promotions.cpp)
target_include_directories(test_type_promotions PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_type_promotions PRIVATE compiler_lib Catch2WithMain)

include(../CTest.cmake OPTIONAL)
add_test(NAME test_type_promotions COMMAND test_type_promotions)
set_tests_properties(test_type_promotions PROPERTIES WILL_FAIL TRUE)
