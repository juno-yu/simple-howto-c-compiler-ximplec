#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "lexer.h"

using namespace simplecc;

TEST_CASE("Empty source", "[tokenizer]") {
    Lexer lexer("");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 1);
    REQUIRE(tokens[0].type == TokenType::EOF_TOKEN);
}

TEST_CASE("Single integer", "[tokenizer]") {
    Lexer lexer("42");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].type == TokenType::INTEGER);
    REQUIRE(tokens[0].value == "42");
    REQUIRE(tokens[1].type == TokenType::EOF_TOKEN);
}

TEST_CASE("Float number", "[tokenizer]") {
    Lexer lexer("3.14");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].type == TokenType::FLOAT);
    REQUIRE(tokens[0].value == "3.14");
}

TEST_CASE("Hex number", "[tokenizer]") {
    Lexer lexer("0xFF");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].type == TokenType::INTEGER);
    REQUIRE(tokens[0].value == "0xFF");
}

TEST_CASE("String literal", "[tokenizer]") {
    Lexer lexer("\"hello\"");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].type == TokenType::STRING_LITERAL);
    REQUIRE(tokens[0].value == "hello");
}

TEST_CASE("String with escape sequences", "[tokenizer]") {
    Lexer lexer("\"line\\nonetwo\"");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].type == TokenType::STRING_LITERAL);
    REQUIRE(tokens[0].value == "line\nonetwo");
}

TEST_CASE("Character literal", "[tokenizer]") {
    Lexer lexer("'a'");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].type == TokenType::CHAR_LITERAL);
    REQUIRE(tokens[0].value == "a");
}

TEST_CASE("Identifier", "[tokenizer]") {
    Lexer lexer("variable_name");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].type == TokenType::IDENTIFIER);
    REQUIRE(tokens[0].value == "variable_name");
}

TEST_CASE("Keywords", "[tokenizer]") {
    Lexer lexer("int void return if else while for break continue");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 10);
    REQUIRE(tokens[0].type == TokenType::KW_INT);
    REQUIRE(tokens[1].type == TokenType::KW_VOID);
    REQUIRE(tokens[2].type == TokenType::KW_RETURN);
    REQUIRE(tokens[3].type == TokenType::KW_IF);
    REQUIRE(tokens[4].type == TokenType::KW_ELSE);
    REQUIRE(tokens[5].type == TokenType::KW_WHILE);
    REQUIRE(tokens[6].type == TokenType::KW_FOR);
    REQUIRE(tokens[7].type == TokenType::KW_BREAK);
    REQUIRE(tokens[8].type == TokenType::KW_CONTINUE);
}

TEST_CASE("Arithmetic operators", "[tokenizer]") {
    Lexer lexer("+ - * / %");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 6);
    REQUIRE(tokens[0].type == TokenType::PLUS);
    REQUIRE(tokens[1].type == TokenType::MINUS);
    REQUIRE(tokens[2].type == TokenType::STAR);
    REQUIRE(tokens[3].type == TokenType::SLASH);
    REQUIRE(tokens[4].type == TokenType::PERCENT);
}

TEST_CASE("Comparison operators", "[tokenizer]") {
    Lexer lexer("== != < > <= >=");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 7);
    REQUIRE(tokens[0].type == TokenType::EQ);
    REQUIRE(tokens[1].type == TokenType::NE);
    REQUIRE(tokens[2].type == TokenType::LT);
    REQUIRE(tokens[3].type == TokenType::GT);
    REQUIRE(tokens[4].type == TokenType::LE);
    REQUIRE(tokens[5].type == TokenType::GE);
}

TEST_CASE("Logical operators", "[tokenizer]") {
    Lexer lexer("&& || !");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 4);
    REQUIRE(tokens[0].type == TokenType::AND);
    REQUIRE(tokens[1].type == TokenType::OR);
    REQUIRE(tokens[2].type == TokenType::NOT);
}

TEST_CASE("Assignment operators", "[tokenizer]") {
    Lexer lexer("= += -= *= /=");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 6);
    REQUIRE(tokens[0].type == TokenType::ASSIGN);
    REQUIRE(tokens[1].type == TokenType::PLUS_ASSIGN);
    REQUIRE(tokens[2].type == TokenType::MINUS_ASSIGN);
    REQUIRE(tokens[3].type == TokenType::STAR_ASSIGN);
    REQUIRE(tokens[4].type == TokenType::SLASH_ASSIGN);
}

TEST_CASE("Increment/Decrement", "[tokenizer]") {
    Lexer lexer("++ --");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0].type == TokenType::PLUS_PLUS);
    REQUIRE(tokens[1].type == TokenType::MINUS_MINUS);
}

TEST_CASE("Delimiters", "[tokenizer]") {
    Lexer lexer("( ) [ ] { } ; , . :");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 11);
    REQUIRE(tokens[0].type == TokenType::LPAREN);
    REQUIRE(tokens[1].type == TokenType::RPAREN);
    REQUIRE(tokens[2].type == TokenType::LBRACKET);
    REQUIRE(tokens[3].type == TokenType::RBRACKET);
    REQUIRE(tokens[4].type == TokenType::LBRACE);
    REQUIRE(tokens[5].type == TokenType::RBRACE);
    REQUIRE(tokens[6].type == TokenType::SEMICOLON);
    REQUIRE(tokens[7].type == TokenType::COMMA);
    REQUIRE(tokens[8].type == TokenType::DOT);
    REQUIRE(tokens[9].type == TokenType::COLON);
}

TEST_CASE("Bitwise operators", "[tokenizer]") {
    Lexer lexer("& | ^ ~ << >>");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 7);
    REQUIRE(tokens[0].type == TokenType::AMPERSAND);
    REQUIRE(tokens[1].type == TokenType::PIPE);
    REQUIRE(tokens[2].type == TokenType::CARET);
    REQUIRE(tokens[3].type == TokenType::TILDE);
    REQUIRE(tokens[4].type == TokenType::LSHIFT);
    REQUIRE(tokens[5].type == TokenType::RSHIFT);
}

TEST_CASE("Simple variable declaration", "[tokenizer]") {
    Lexer lexer("int x = 42;");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 6);
    REQUIRE(tokens[0].type == TokenType::KW_INT);
    REQUIRE(tokens[1].type == TokenType::IDENTIFIER);
    REQUIRE(tokens[1].value == "x");
    REQUIRE(tokens[2].type == TokenType::ASSIGN);
    REQUIRE(tokens[3].type == TokenType::INTEGER);
    REQUIRE(tokens[3].value == "42");
    REQUIRE(tokens[4].type == TokenType::SEMICOLON);
}

TEST_CASE("Function declaration", "[tokenizer]") {
    Lexer lexer("int main() { return 0; }");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 10);
    REQUIRE(tokens[0].type == TokenType::KW_INT);
    REQUIRE(tokens[1].type == TokenType::IDENTIFIER);
    REQUIRE(tokens[1].value == "main");
    REQUIRE(tokens[2].type == TokenType::LPAREN);
    REQUIRE(tokens[3].type == TokenType::RPAREN);
    REQUIRE(tokens[4].type == TokenType::LBRACE);
    REQUIRE(tokens[5].type == TokenType::KW_RETURN);
    REQUIRE(tokens[6].type == TokenType::INTEGER);
    REQUIRE(tokens[6].value == "0");
    REQUIRE(tokens[7].type == TokenType::SEMICOLON);
    REQUIRE(tokens[8].type == TokenType::RBRACE);
}

TEST_CASE("Whitespace handling", "[tokenizer]") {
    Lexer lexer("  int   x   =   42  ;  ");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 6);
    REQUIRE(tokens[0].type == TokenType::KW_INT);
    REQUIRE(tokens[1].type == TokenType::IDENTIFIER);
    REQUIRE(tokens[2].type == TokenType::ASSIGN);
    REQUIRE(tokens[3].type == TokenType::INTEGER);
    REQUIRE(tokens[4].type == TokenType::SEMICOLON);
}

TEST_CASE("Line comments", "[tokenizer]") {
    Lexer lexer("int x; // this is a comment\nint y;");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 7);
    REQUIRE(tokens[0].type == TokenType::KW_INT);
    REQUIRE(tokens[1].type == TokenType::IDENTIFIER);
    REQUIRE(tokens[2].type == TokenType::SEMICOLON);
    REQUIRE(tokens[3].type == TokenType::KW_INT);
    REQUIRE(tokens[4].type == TokenType::IDENTIFIER);
    REQUIRE(tokens[5].type == TokenType::SEMICOLON);
}

TEST_CASE("Block comments", "[tokenizer]") {
    Lexer lexer("int /* comment */ x;");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 4);
    REQUIRE(tokens[0].type == TokenType::KW_INT);
    REQUIRE(tokens[1].type == TokenType::IDENTIFIER);
    REQUIRE(tokens[1].value == "x");
    REQUIRE(tokens[2].type == TokenType::SEMICOLON);
}

TEST_CASE("Line tracking", "[tokenizer]") {
    Lexer lexer("int x;\nint y;");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 7);
    REQUIRE(tokens[0].line == 1);
    REQUIRE(tokens[3].line == 2);
}

TEST_CASE("Error: unterminated string", "[tokenizer]") {
    Lexer lexer("\"unterminated");
    auto tokens = lexer.tokenize();
    REQUIRE(lexer.has_error());
    REQUIRE(lexer.error_line() == 1);
}

TEST_CASE("Error: unterminated block comment", "[tokenizer]") {
    Lexer lexer("/* unterminated");
    auto tokens = lexer.tokenize();
    REQUIRE(lexer.has_error());
}

TEST_CASE("Error: invalid character", "[tokenizer]") {
    Lexer lexer("@");
    auto tokens = lexer.tokenize();
    REQUIRE(lexer.has_error());
}

TEST_CASE("Complex expression", "[tokenizer]") {
    Lexer lexer("result = (a + b) * c - d / e;");
    auto tokens = lexer.tokenize();
    REQUIRE(tokens.size() == 15);
    REQUIRE(tokens[0].type == TokenType::IDENTIFIER);
    REQUIRE(tokens[1].type == TokenType::ASSIGN);
    REQUIRE(tokens[2].type == TokenType::LPAREN);
    REQUIRE(tokens[3].type == TokenType::IDENTIFIER);
    REQUIRE(tokens[4].type == TokenType::PLUS);
    REQUIRE(tokens[5].type == TokenType::IDENTIFIER);
    REQUIRE(tokens[6].type == TokenType::RPAREN);
    REQUIRE(tokens[7].type == TokenType::STAR);
    REQUIRE(tokens[8].type == TokenType::IDENTIFIER);
    REQUIRE(tokens[9].type == TokenType::MINUS);
    REQUIRE(tokens[10].type == TokenType::IDENTIFIER);
    REQUIRE(tokens[11].type == TokenType::SLASH);
    REQUIRE(tokens[12].type == TokenType::IDENTIFIER);
    REQUIRE(tokens[13].type == TokenType::SEMICOLON);
}
