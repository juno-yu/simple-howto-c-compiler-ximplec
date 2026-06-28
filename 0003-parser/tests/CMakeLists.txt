add_executable(test_parser test_parser.cpp)
target_link_libraries(test_parser compiler_lib Catch2::Catch2WithMain)

add_test(NAME parser_tests COMMAND test_parser)
