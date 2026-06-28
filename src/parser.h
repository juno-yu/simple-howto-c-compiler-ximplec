#pragma once

#include "ast.h"
#include "token.h"
#include <vector>
#include <string>
#include <map>
#include <set>

namespace simplecc {

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    ~Parser() = default;
    
    ASTPtr parse();
    
    bool has_error() const { return !error_message_.empty(); }
    const std::string& error_message() const { return error_message_; }
    int error_line() const { return error_line_; }
    int error_column() const { return error_column_; }
    
private:
    // Token management
    const Token& peek() const;
    const Token& peek_next() const;
    const Token& advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    bool expect(TokenType type);
    
    // Grammar rules
    ASTPtr parse_program();
    ASTPtr parse_declaration();
    ASTPtr parse_function_decl(const std::string& type_name);
    ASTPtr parse_var_decl(const std::string& type_name);
    ASTPtr parse_struct_decl();
    ASTPtr parse_enum_decl();
    ASTPtr parse_typedef_decl();
    ASTPtr parse_switch_stmt();
    ASTPtr parse_goto_stmt();
    ASTPtr parse_param();
    ASTPtr parse_block();
    ASTPtr parse_statement();
    ASTPtr parse_return_stmt();
    ASTPtr parse_expr_stmt();
    ASTPtr parse_if_stmt();
    ASTPtr parse_while_stmt();
    ASTPtr parse_do_while_stmt();
    ASTPtr parse_for_stmt();
    
    // Expression parsing (precedence climbing)
    ASTPtr parse_expression();
    ASTPtr parse_assignment();
    ASTPtr parse_or();
    ASTPtr parse_and();
    ASTPtr parse_bitwise_or();
    ASTPtr parse_bitwise_xor();
    ASTPtr parse_bitwise_and();
    ASTPtr parse_equality();
    ASTPtr parse_comparison();
    ASTPtr parse_shift();
    ASTPtr parse_addition();
    ASTPtr parse_multiplication();
    ASTPtr parse_unary();
    ASTPtr parse_postfix();
    ASTPtr parse_primary();
    
    // Helpers
    bool is_at_end() const;
    bool is_type_specifier() const;
    std::string parse_type_specifier();
    void error(const std::string& message);
    
    std::vector<Token> tokens_;
    size_t pos_;
    std::string error_message_;
    int error_line_;
    int error_column_;
    
    // Enum values for constant folding
    std::map<std::string, long long> enum_constants_;
    
    // Typedef names for type specifier recognition
    std::set<std::string> typedef_names_;
};

} // namespace simplecc
