#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace simplecc {

enum class TokenType {
    // Literals
    INTEGER,
    FLOAT,
    STRING_LITERAL,
    CHAR_LITERAL,
    
    // Identifiers
    IDENTIFIER,
    
    // Keywords
    KW_INT,
    KW_CHAR,
    KW_VOID,
    KW_RETURN,
    KW_IF,
    KW_ELSE,
    KW_WHILE,
    KW_FOR,
    KW_BREAK,
    KW_CONTINUE,
    KW_DO,
    KW_BOOL,
    KW_CONST,
    KW_EXTERN,
    KW_STRUCT,
    KW_SIZEOF,
    KW_SWITCH,
    KW_CASE,
    KW_DEFAULT,
    KW_ENUM,
    KW_TYPEDEF,
    KW_STATIC,
    KW_UNSIGNED,
    KW_SIGNED,
    KW_LONG,
    KW_SHORT,
    KW_FLOAT,
    KW_DOUBLE,
    KW_GOTO,
    KW_VOLATILE,
    KW_UNION,
    KW_INLINE,
    KW_REGISTER,
    KW_AUTO,
    KW_RESTRICT,
    KW_THREAD_LOCAL,
    KW_ATOMIC,
    KW_STATIC_ASSERT,
    KW_GENERIC,
    KW_ALIGNOF,
    KW_ALIGNAS,
    ELLIPSIS,       // ...
    
    // Operators
    PLUS,           // +
    MINUS,          // -
    STAR,           // *
    SLASH,          // /
    PERCENT,        // %
    ASSIGN,         // =
    PLUS_ASSIGN,    // +=
    MINUS_ASSIGN,   // -=
    STAR_ASSIGN,    // *=
    SLASH_ASSIGN,   // /=
    EQ,             // ==
    NE,             // !=
    LT,             // <
    GT,             // >
    LE,             // <=
    GE,             // >=
    AND,            // &&
    OR,             // ||
    NOT,            // !
    AMPERSAND,      // &
    PIPE,           // |
    CARET,          // ^
    TILDE,          // ~
    LSHIFT,         // <<
    RSHIFT,         // >>
    PLUS_PLUS,      // ++
    MINUS_MINUS,    // --
    MINUS_GT,       // ->
    QUESTION,       // ?
    
    // Delimiters
    SEMICOLON,      // ;
    COMMA,          // ,
    DOT,            // .
    COLON,          // :
    LPAREN,         // (
    RPAREN,         // )
    LBRACKET,       // [
    RBRACKET,       // ]
    LBRACE,         // {
    RBRACE,         // }
    
    // Special
    EOF_TOKEN,
    NEWLINE
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
    
    Token() : type(TokenType::EOF_TOKEN), line(0), column(0) {}
    Token(TokenType t, const std::string& v, int l, int c)
        : type(t), value(v), line(l), column(c) {}
};

const char* token_type_name(TokenType type);

} // namespace simplecc
