add_executable(test_lexer test_lexer.cpp)
target_link_libraries(test_lexer compiler_lib Catch2::Catch2WithMain)

add_test(NAME tokenizer_tests COMMAND test_lexer)
