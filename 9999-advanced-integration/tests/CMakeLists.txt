add_executable(test_advanced test_advanced.cpp)
target_link_libraries(test_advanced compiler_lib Catch2::Catch2WithMain)

add_test(NAME test_advanced COMMAND test_advanced)
