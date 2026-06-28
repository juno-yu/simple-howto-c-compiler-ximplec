add_executable(test_ast test_ast.cpp)
target_link_libraries(test_ast compiler_lib Catch2::Catch2WithMain)

add_test(NAME ast_tests COMMAND test_ast)
