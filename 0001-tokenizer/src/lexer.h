#pragma once

#include "token.h"
#include <string>
#include <vector>
#include <functional>

namespace simplecc {

class Lexer {
public:
    Lexer(const std::string& source);
    ~Lexer() = default;
    
    std::vector<Token> tokenize();
    Token next_token();
    
    bool has_error() const { return !error_message_.empty(); }
    const std::string& error_message() const { return error_message_; }
    int error_line() const { return error_line_; }
    int error_column() const { return error_column_; }
    
private:
    char peek() const;
    char peek_next() const;
    char advance();
    bool match(char expected);
    
    void skip_whitespace();
    void skip_comment();
    void skip_line_comment();
    void skip_block_comment();
    
    Token read_number();
    Token read_string();
    Token read_char();
    Token read_identifier();
    Token read_operator();
    
    Token make_token(TokenType type, const std::string& value);
    Token error_token(const std::string& message);
    
    bool is_at_end() const;
    
    static const std::unordered_map<std::string, TokenType>& keywords();
    
    std::string source_;
    size_t pos_;
    int line_;
    int column_;
    std::string error_message_;
    int error_line_;
    int error_column_;
};

} // namespace simplecc
