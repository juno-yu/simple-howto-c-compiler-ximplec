#include "lexer.h"
#include <cctype>
#include <stdexcept>

namespace simplecc {

const char* token_type_name(TokenType type) {
    switch (type) {
        case TokenType::INTEGER: return "INTEGER";
        case TokenType::FLOAT: return "FLOAT";
        case TokenType::STRING_LITERAL: return "STRING_LITERAL";
        case TokenType::CHAR_LITERAL: return "CHAR_LITERAL";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::KW_INT: return "int";
        case TokenType::KW_CHAR: return "char";
        case TokenType::KW_VOID: return "void";
        case TokenType::KW_RETURN: return "return";
        case TokenType::KW_IF: return "if";
        case TokenType::KW_ELSE: return "else";
        case TokenType::KW_WHILE: return "while";
        case TokenType::KW_FOR: return "for";
        case TokenType::KW_BREAK: return "break";
        case TokenType::KW_CONTINUE: return "continue";
        case TokenType::KW_SIZEOF: return "sizeof";
        case TokenType::KW_SWITCH: return "switch";
        case TokenType::KW_CASE: return "case";
        case TokenType::KW_DEFAULT: return "default";
        case TokenType::KW_ENUM: return "enum";
        case TokenType::KW_TYPEDEF: return "typedef";
        case TokenType::KW_STATIC: return "static";
        case TokenType::KW_UNSIGNED: return "unsigned";
        case TokenType::KW_SIGNED: return "signed";
        case TokenType::KW_LONG: return "long";
        case TokenType::KW_SHORT: return "short";
        case TokenType::KW_FLOAT: return "float";
        case TokenType::KW_DOUBLE: return "double";
        case TokenType::KW_GOTO: return "goto";
        case TokenType::KW_VOLATILE: return "volatile";
        case TokenType::KW_UNION: return "union";
        case TokenType::KW_INLINE: return "inline";
        case TokenType::ELLIPSIS: return "...";
        case TokenType::QUESTION: return "?";
        case TokenType::MINUS_GT: return "->";
        case TokenType::PLUS: return "+";
        case TokenType::MINUS: return "-";
        case TokenType::STAR: return "*";
        case TokenType::SLASH: return "/";
        case TokenType::PERCENT: return "%";
        case TokenType::ASSIGN: return "=";
        case TokenType::PLUS_ASSIGN: return "+=";
        case TokenType::MINUS_ASSIGN: return "-=";
        case TokenType::STAR_ASSIGN: return "*=";
        case TokenType::SLASH_ASSIGN: return "/=";
        case TokenType::EQ: return "==";
        case TokenType::NE: return "!=";
        case TokenType::LT: return "<";
        case TokenType::GT: return ">";
        case TokenType::LE: return "<=";
        case TokenType::GE: return ">=";
        case TokenType::AND: return "&&";
        case TokenType::OR: return "||";
        case TokenType::NOT: return "!";
        case TokenType::AMPERSAND: return "&";
        case TokenType::PIPE: return "|";
        case TokenType::CARET: return "^";
        case TokenType::TILDE: return "~";
        case TokenType::LSHIFT: return "<<";
        case TokenType::RSHIFT: return ">>";
        case TokenType::PLUS_PLUS: return "++";
        case TokenType::MINUS_MINUS: return "--";
        case TokenType::SEMICOLON: return ";";
        case TokenType::COMMA: return ",";
        case TokenType::DOT: return ".";
        case TokenType::COLON: return ":";
        case TokenType::LPAREN: return "(";
        case TokenType::RPAREN: return ")";
        case TokenType::LBRACKET: return "[";
        case TokenType::RBRACKET: return "]";
        case TokenType::LBRACE: return "{";
        case TokenType::RBRACE: return "}";
        case TokenType::EOF_TOKEN: return "EOF";
        case TokenType::NEWLINE: return "NEWLINE";
    }
    return "UNKNOWN";
}

Lexer::Lexer(const std::string& source)
    : source_(source), pos_(0), line_(1), column_(1),
      error_line_(0), error_column_(0) {}

const std::unordered_map<std::string, TokenType>& Lexer::keywords() {
    static const std::unordered_map<std::string, TokenType> kw = {
        {"int", TokenType::KW_INT},
        {"char", TokenType::KW_CHAR},
        {"void", TokenType::KW_VOID},
        {"return", TokenType::KW_RETURN},
        {"if", TokenType::KW_IF},
        {"else", TokenType::KW_ELSE},
        {"while", TokenType::KW_WHILE},
        {"for", TokenType::KW_FOR},
        {"break", TokenType::KW_BREAK},
        {"continue", TokenType::KW_CONTINUE},
        {"do", TokenType::KW_DO},
        {"bool", TokenType::KW_BOOL},
        {"const", TokenType::KW_CONST},
        {"extern", TokenType::KW_EXTERN},
        {"struct", TokenType::KW_STRUCT},
        {"sizeof", TokenType::KW_SIZEOF},
        {"switch", TokenType::KW_SWITCH},
        {"case", TokenType::KW_CASE},
        {"default", TokenType::KW_DEFAULT},
        {"enum", TokenType::KW_ENUM},
        {"typedef", TokenType::KW_TYPEDEF},
        {"static", TokenType::KW_STATIC},
        {"unsigned", TokenType::KW_UNSIGNED},
        {"signed", TokenType::KW_SIGNED},
        {"long", TokenType::KW_LONG},
        {"short", TokenType::KW_SHORT},
        {"float", TokenType::KW_FLOAT},
        {"double", TokenType::KW_DOUBLE},
        {"goto", TokenType::KW_GOTO},
        {"volatile", TokenType::KW_VOLATILE},
        {"union", TokenType::KW_UNION},
        {"inline", TokenType::KW_INLINE},
    };
    return kw;
}

char Lexer::peek() const {
    if (pos_ >= source_.size()) return '\0';
    return source_[pos_];
}

char Lexer::peek_next() const {
    if (pos_ + 1 >= source_.size()) return '\0';
    return source_[pos_ + 1];
}

char Lexer::advance() {
    char c = source_[pos_++];
    if (c == '\n') {
        line_++;
        column_ = 1;
    } else {
        column_++;
    }
    return c;
}

bool Lexer::match(char expected) {
    if (is_at_end() || source_[pos_] != expected) return false;
    advance();
    return true;
}

bool Lexer::is_at_end() const {
    return pos_ >= source_.size();
}

void Lexer::skip_whitespace() {
    while (!is_at_end()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r') {
            advance();
        } else if (c == '\n') {
            advance();
        } else if (c == '/' && peek_next() == '/') {
            skip_line_comment();
        } else if (c == '/' && peek_next() == '*') {
            skip_block_comment();
        } else {
            break;
        }
    }
}

void Lexer::skip_comment() {
    if (peek() == '/' && peek_next() == '/') {
        skip_line_comment();
    } else if (peek() == '/' && peek_next() == '*') {
        skip_block_comment();
    }
}

void Lexer::skip_line_comment() {
    advance(); // skip first /
    advance(); // skip second /
    while (!is_at_end() && peek() != '\n') {
        advance();
    }
}

void Lexer::skip_block_comment() {
    advance(); // skip /
    advance(); // skip *
    while (!is_at_end()) {
        if (peek() == '*' && peek_next() == '/') {
            advance(); // skip *
            advance(); // skip /
            return;
        }
        advance();
    }
    error_message_ = "Unterminated block comment";
    error_line_ = line_;
    error_column_ = column_;
}

Token Lexer::read_number() {
    int start_line = line_;
    int start_column = column_;
    std::string num;
    
    bool is_float = false;
    
    while (!is_at_end() && std::isdigit(peek())) {
        num += advance();
    }
    
    if (!is_at_end() && peek() == '.' && std::isdigit(peek_next())) {
        is_float = true;
        num += advance(); // skip '.'
        while (!is_at_end() && std::isdigit(peek())) {
            num += advance();
        }
    }
    
    // Handle hex numbers
    if (num.size() == 1 && num[0] == '0' && !is_at_end() && 
        (peek() == 'x' || peek() == 'X')) {
        num += advance(); // skip 'x'
        while (!is_at_end() && std::isxdigit(peek())) {
            num += advance();
        }
    }
    
    if (!is_at_end() && (peek() == 'l' || peek() == 'L' || peek() == 'f' || peek() == 'F')) {
        advance(); // skip suffix
    }
    
    if (is_float) {
        return Token(TokenType::FLOAT, num, start_line, start_column);
    }
    return Token(TokenType::INTEGER, num, start_line, start_column);
}

Token Lexer::read_string() {
    int start_line = line_;
    int start_column = column_;
    advance(); // skip opening quote
    
    std::string str;
    while (!is_at_end() && peek() != '"') {
        if (peek() == '\\') {
            advance();
            if (is_at_end()) break;
            char c = advance();
            switch (c) {
                case 'n': str += '\n'; break;
                case 't': str += '\t'; break;
                case '\\': str += '\\'; break;
                case '"': str += '"'; break;
                case '0': str += '\0'; break;
                default: str += c; break;
            }
        } else if (peek() == '\n') {
            error_message_ = "Unterminated string literal";
            error_line_ = line_;
            error_column_ = column_;
            return error_token("Unterminated string literal");
        } else {
            str += advance();
        }
    }
    
    if (is_at_end()) {
        error_message_ = "Unterminated string literal";
        error_line_ = line_;
        error_column_ = column_;
        return error_token("Unterminated string literal");
    }
    
    advance(); // skip closing quote
    return Token(TokenType::STRING_LITERAL, str, start_line, start_column);
}

Token Lexer::read_char() {
    int start_line = line_;
    int start_column = column_;
    advance(); // skip opening quote
    
    std::string ch;
    if (peek() == '\\') {
        advance();
        if (!is_at_end()) {
            char c = advance();
            switch (c) {
                case 'n': ch = "\n"; break;
                case 't': ch = "\t"; break;
                case '\\': ch = "\\"; break;
                case '\'': ch = "'"; break;
                case '0': ch = "\0"; break;
                default: ch = std::string(1, c); break;
            }
        }
    } else if (!is_at_end()) {
        ch = std::string(1, advance());
    }
    
    if (is_at_end() || peek() != '\'') {
        error_message_ = "Unterminated character literal";
        error_line_ = line_;
        error_column_ = column_;
        return error_token("Unterminated character literal");
    }
    
    advance(); // skip closing quote
    return Token(TokenType::CHAR_LITERAL, ch, start_line, start_column);
}

Token Lexer::read_identifier() {
    int start_line = line_;
    int start_column = column_;
    std::string id;
    
    while (!is_at_end() && (std::isalnum(peek()) || peek() == '_')) {
        id += advance();
    }
    
    auto& kws = keywords();
    auto it = kws.find(id);
    if (it != kws.end()) {
        return Token(it->second, id, start_line, start_column);
    }
    
    return Token(TokenType::IDENTIFIER, id, start_line, start_column);
}

Token Lexer::read_operator() {
    int start_line = line_;
    int start_column = column_;
    char c = advance();
    
    switch (c) {
        case '+':
            if (match('+')) return Token(TokenType::PLUS_PLUS, "++", start_line, start_column);
            if (match('=')) return Token(TokenType::PLUS_ASSIGN, "+=", start_line, start_column);
            return Token(TokenType::PLUS, "+", start_line, start_column);
        case '-':
            if (match('-')) return Token(TokenType::MINUS_MINUS, "--", start_line, start_column);
            if (match('=')) return Token(TokenType::MINUS_ASSIGN, "-=", start_line, start_column);
            return Token(TokenType::MINUS, "-", start_line, start_column);
        case '*':
            if (match('=')) return Token(TokenType::STAR_ASSIGN, "*=", start_line, start_column);
            return Token(TokenType::STAR, "*", start_line, start_column);
        case '/':
            if (match('=')) return Token(TokenType::SLASH_ASSIGN, "/=", start_line, start_column);
            return Token(TokenType::SLASH, "/", start_line, start_column);
        case '%':
            return Token(TokenType::PERCENT, "%", start_line, start_column);
        case '=':
            if (match('=')) return Token(TokenType::EQ, "==", start_line, start_column);
            return Token(TokenType::ASSIGN, "=", start_line, start_column);
        case '!':
            if (match('=')) return Token(TokenType::NE, "!=", start_line, start_column);
            return Token(TokenType::NOT, "!", start_line, start_column);
        case '?':
            return Token(TokenType::QUESTION, "?", start_line, start_column);
        case '<':
            if (match('<')) return Token(TokenType::LSHIFT, "<<", start_line, start_column);
            if (match('=')) return Token(TokenType::LE, "<=", start_line, start_column);
            return Token(TokenType::LT, "<", start_line, start_column);
        case '>':
            if (match('>')) return Token(TokenType::RSHIFT, ">>", start_line, start_column);
            if (match('=')) return Token(TokenType::GE, ">=", start_line, start_column);
            return Token(TokenType::GT, ">", start_line, start_column);
        case '&':
            if (match('&')) return Token(TokenType::AND, "&&", start_line, start_column);
            return Token(TokenType::AMPERSAND, "&", start_line, start_column);
        case '|':
            if (match('|')) return Token(TokenType::OR, "||", start_line, start_column);
            return Token(TokenType::PIPE, "|", start_line, start_column);
        case '^':
            return Token(TokenType::CARET, "^", start_line, start_column);
        case '~':
            return Token(TokenType::TILDE, "~", start_line, start_column);
        case '(':
            return Token(TokenType::LPAREN, "(", start_line, start_column);
        case ')':
            return Token(TokenType::RPAREN, ")", start_line, start_column);
        case '[':
            return Token(TokenType::LBRACKET, "[", start_line, start_column);
        case ']':
            return Token(TokenType::RBRACKET, "]", start_line, start_column);
        case '{':
            return Token(TokenType::LBRACE, "{", start_line, start_column);
        case '}':
            return Token(TokenType::RBRACE, "}", start_line, start_column);
        case ';':
            return Token(TokenType::SEMICOLON, ";", start_line, start_column);
        case ',':
            return Token(TokenType::COMMA, ",", start_line, start_column);
        case '.':
            if (match('.') && match('.')) return Token(TokenType::ELLIPSIS, "...", start_line, start_column);
            return Token(TokenType::DOT, ".", start_line, start_column);
        case ':':
            return Token(TokenType::COLON, ":", start_line, start_column);
        default:
            return error_token(std::string("Unexpected character: ") + c);
    }
}

Token Lexer::make_token(TokenType type, const std::string& value) {
    return Token(type, value, line_, column_);
}

Token Lexer::error_token(const std::string& message) {
    error_message_ = message;
    error_line_ = line_;
    error_column_ = column_;
    return Token(TokenType::EOF_TOKEN, message, line_, column_);
}

Token Lexer::next_token() {
    skip_whitespace();
    
    if (is_at_end()) {
        return Token(TokenType::EOF_TOKEN, "", line_, column_);
    }
    
    char c = peek();
    
    if (std::isdigit(c)) {
        return read_number();
    }
    
    if (c == '"') {
        return read_string();
    }
    
    if (c == '\'') {
        return read_char();
    }
    
    if (std::isalpha(c) || c == '_') {
        return read_identifier();
    }
    
    return read_operator();
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    while (true) {
        Token token = next_token();
        tokens.push_back(token);
        
        if (token.type == TokenType::EOF_TOKEN) {
            break;
        }
        
        if (!error_message_.empty()) {
            break;
        }
    }
    
    return tokens;
}

} // namespace simplecc
