add_executable(test_codegen test_codegen.cpp)
target_link_libraries(test_codegen compiler_lib Catch2::Catch2WithMain)

add_test(NAME codegen_tests COMMAND test_codegen)
