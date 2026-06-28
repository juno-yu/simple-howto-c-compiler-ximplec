#include "parser.h"
#include <stdexcept>

namespace simplecc {

Parser::Parser(const std::vector<Token>& tokens)
    : tokens_(tokens), pos_(0), error_line_(0), error_column_(0) {
    // Add sentinel EOF token if not present
    if (tokens_.empty() || tokens_.back().type != TokenType::EOF_TOKEN) {
        tokens_.push_back(Token(TokenType::EOF_TOKEN, "", 0, 0));
    }
}

const Token& Parser::peek() const {
    return tokens_[pos_];
}

const Token& Parser::peek_next() const {
    if (pos_ + 1 >= tokens_.size()) {
        return tokens_.back();
    }
    return tokens_[pos_ + 1];
}

const Token& Parser::advance() {
    if (!is_at_end()) {
        pos_++;
    }
    return tokens_[pos_ - 1];
}

bool Parser::check(TokenType type) const {
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::expect(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    error("Expected " + std::string(token_type_name(type)) + 
          " but found " + std::string(token_type_name(peek().type)));
    return false;
}

bool Parser::is_at_end() const {
    return peek().type == TokenType::EOF_TOKEN;
}

bool Parser::is_type_specifier() const {
    if (check(TokenType::KW_INT) || 
        check(TokenType::KW_CHAR) || 
        check(TokenType::KW_VOID) ||
        check(TokenType::KW_BOOL) ||
        check(TokenType::KW_CONST) ||
        check(TokenType::KW_STRUCT) ||
        check(TokenType::KW_ENUM) ||
        check(TokenType::KW_TYPEDEF) ||
        check(TokenType::KW_UNSIGNED) ||
        check(TokenType::KW_SIGNED) ||
        check(TokenType::KW_LONG) ||
        check(TokenType::KW_SHORT) ||
        check(TokenType::KW_FLOAT) ||
        check(TokenType::KW_DOUBLE) ||
        check(TokenType::KW_VOLATILE) ||
        check(TokenType::KW_STATIC) ||
        check(TokenType::KW_EXTERN) ||
        check(TokenType::KW_UNION) ||
        check(TokenType::KW_INLINE) ||
        check(TokenType::KW_REGISTER) ||
        check(TokenType::KW_AUTO) ||
        check(TokenType::KW_RESTRICT) ||
        check(TokenType::KW_THREAD_LOCAL) ||
        check(TokenType::KW_ATOMIC)) {
        return true;
    }
    // Check if current identifier is a typedef name
    if (check(TokenType::IDENTIFIER) && typedef_names_.count(peek().value)) {
        return true;
    }
    return false;
}

std::string Parser::parse_type_specifier() {
    std::string result;
    
    // Handle qualifiers and storage class (in any order)
    while (true) {
        if (match(TokenType::KW_CONST)) {
            result += "const ";
        } else if (match(TokenType::KW_VOLATILE)) {
            result += "volatile ";
        } else if (match(TokenType::KW_STATIC)) {
            result += "static ";
        } else if (match(TokenType::KW_EXTERN)) {
            result += "extern ";
        } else if (match(TokenType::KW_INLINE)) {
            result += "inline ";
        } else if (match(TokenType::KW_REGISTER)) {
            result += "register ";
        } else if (match(TokenType::KW_AUTO)) {
            result += "auto ";
        } else if (match(TokenType::KW_RESTRICT)) {
            result += "restrict ";
        } else if (match(TokenType::KW_THREAD_LOCAL)) {
            result += "_Thread_local ";
        } else if (match(TokenType::KW_ATOMIC)) {
            result += "_Atomic ";
        } else if (match(TokenType::KW_ALIGNAS)) {
            // _Alignas(N) - skip the alignment specifier
            if (match(TokenType::LPAREN)) {
                // Skip alignment expression
                parse_expression();
                expect(TokenType::RPAREN);
            }
        } else if (match(TokenType::KW_ATTRIBUTE)) {
            // __attribute__((...)) — skip entirely
            if (match(TokenType::LPAREN)) {
                if (match(TokenType::LPAREN)) {
                    // Skip until ))
                    int depth = 1;
                    while (depth > 0 && !is_at_end()) {
                        if (check(TokenType::LPAREN)) depth++;
                        if (check(TokenType::RPAREN)) depth--;
                        if (depth > 0) advance();
                    }
                    if (!is_at_end()) advance(); // consume )
                }
                if (!is_at_end()) advance(); // consume )
            }
        } else {
            break;
        }
    }
    
    // Handle signed/unsigned/long/short modifiers
    bool is_unsigned = false;
    bool is_signed = false;
    int long_count = 0;
    int short_count = 0;
    
    if (match(TokenType::KW_UNSIGNED)) { is_unsigned = true; result += "unsigned "; }
    else if (match(TokenType::KW_SIGNED)) { is_signed = true; result += "signed "; }
    
    while (match(TokenType::KW_LONG)) { long_count++; result += "long "; }
    while (match(TokenType::KW_SHORT)) { short_count++; result += "short "; }
    
    // If we had unsigned/signed but no base type yet, look for one
    if ((is_unsigned || is_signed) && !long_count && !short_count) {
        // Could be just `unsigned` alone (means unsigned int)
        // Or followed by int/char
    }
    
    if (match(TokenType::KW_INT)) {
        result += "int";
    } else if (match(TokenType::KW_CHAR)) {
        result += "char";
    } else if (match(TokenType::KW_VOID)) {
        result += "void";
    } else if (match(TokenType::KW_BOOL)) {
        result += "bool";
    } else if (match(TokenType::KW_FLOAT)) {
        result += "float";
    } else if (match(TokenType::KW_DOUBLE)) {
        result += "double";
    } else if (match(TokenType::KW_STRUCT)) {
        if (check(TokenType::IDENTIFIER)) {
            result += "struct " + peek().value;
            advance();
        } else if (check(TokenType::LBRACE)) {
            // Anonymous struct — generate a synthetic name
            result += "struct _anon_" + std::to_string(pos_);
        } else {
            error("Expected struct name");
            return result;
        }
    } else if (match(TokenType::KW_UNION)) {
        if (check(TokenType::IDENTIFIER)) {
            result += "union " + peek().value;
            advance();
        } else if (check(TokenType::LBRACE)) {
            result += "union _anon_" + std::to_string(pos_);
        } else {
            error("Expected union name");
            return result;
        }
    } else if (match(TokenType::KW_ENUM)) {
        if (!check(TokenType::IDENTIFIER)) {
            error("Expected enum name");
            return result;
        }
        result += "enum " + peek().value;
        advance();
    } else if (match(TokenType::KW_TYPEDEF)) {
        // typedef name used as type - just return the name
        if (!check(TokenType::IDENTIFIER)) {
            error("Expected typedef name");
            return result;
        }
        result += peek().value;
        advance();
    } else if (result.empty() || is_unsigned || is_signed || long_count || short_count) {
        // Check if this is a typedef name
        if (check(TokenType::IDENTIFIER) && typedef_names_.count(peek().value)) {
            result += peek().value;
            advance();
        } else if (result.empty()) {
            error("Expected type specifier");
        }
        // If we have unsigned/signed/long/short but no base type, that's OK (means int)
    }
    
    // Handle pointer qualifiers (*)
    while (match(TokenType::STAR)) {
        result += "*";
        // Skip qualifiers after * (e.g., int * const restrict p)
        while (check(TokenType::KW_CONST) || check(TokenType::KW_VOLATILE) ||
               check(TokenType::KW_RESTRICT) || check(TokenType::KW_ATOMIC)) {
            advance(); // skip qualifier
        }
    }
    
    return result;
}

void Parser::error(const std::string& message) {
    if (error_message_.empty()) {
        error_message_ = message + " at line " + std::to_string(peek().line) +
                        ", column " + std::to_string(peek().column);
        error_line_ = peek().line;
        error_column_ = peek().column;
    }
}

ASTPtr Parser::parse() {
    return parse_program();
}

ASTPtr Parser::parse_program() {
    auto program = std::make_unique<ProgramNode>();
    
    while (!is_at_end()) {
        ASTPtr decl;
        if (is_type_specifier()) {
            decl = parse_declaration();
        } else {
            decl = parse_statement();
        }
        if (decl) {
            program->declarations.push_back(std::move(decl));
        } else {
            break;
        }
    }
    
    return std::move(program);
}

ASTPtr Parser::parse_declaration() {
    // Handle extern keyword
    if (match(TokenType::KW_EXTERN)) {
        // extern declarations - just parse as forward declarations
        std::string type_name = parse_type_specifier();
        if (!check(TokenType::IDENTIFIER)) {
            error("Expected identifier after extern");
            return nullptr;
        }
        const Token& name_token = peek();
        std::string name = name_token.value;
        advance();
        
        auto func = std::make_unique<FunctionDeclNode>(
            type_name, name, name_token.line, name_token.column);
        
        if (match(TokenType::LPAREN)) {
            // Function parameters
            if (!check(TokenType::RPAREN)) {
                do {
                    auto param = parse_param();
                    if (param) {
                        func->params.push_back(std::move(param));
                    }
                } while (match(TokenType::COMMA));
            }
            expect(TokenType::RPAREN);
        }
        
        expect(TokenType::SEMICOLON);
        func->body = nullptr; // extern = no body
        return std::move(func);
    }
    
    // Handle struct keyword
    if (match(TokenType::KW_STRUCT)) {
        // Check if this is a struct definition (has {) or just a type usage
        if (check(TokenType::LBRACE)) {
            // Anonymous struct - error, need a name
            error("Expected struct name");
            return nullptr;
        }
        if (!check(TokenType::IDENTIFIER)) {
            error("Expected struct name");
            return nullptr;
        }
        // Save position - peek at what follows the struct name
        std::string struct_name = peek().value;
        advance(); // consume struct name
        
        if (check(TokenType::LBRACE)) {
            // This is a struct definition: struct Name { ... };
            // We already consumed the name, build the struct decl
            auto struct_decl = std::make_unique<StructDeclNode>(
                struct_name, tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
            
            expect(TokenType::LBRACE);
            while (!check(TokenType::RBRACE) && !is_at_end()) {
                std::string field_type = parse_type_specifier();
                
                // Handle anonymous struct/union: struct { ... } inside a struct
                if (check(TokenType::LBRACE)) {
                    int depth = 1;
                    advance(); // consume {
                    while (depth > 0 && !is_at_end()) {
                        if (check(TokenType::LBRACE)) depth++;
                        if (check(TokenType::RBRACE) && depth > 0) depth--;
                        if (depth > 0) advance();
                    }
                    if (!is_at_end()) advance(); // consume }
                    // Skip optional field name after the anonymous struct
                    if (check(TokenType::IDENTIFIER)) {
                        const Token& aname = peek();
                        std::string afname = aname.value;
                        advance();
                        auto afield = std::make_unique<StructFieldNode>(
                            field_type, afname, aname.line, aname.column);
                        struct_decl->fields.push_back(std::move(afield));
                    }
                    if (match(TokenType::SEMICOLON)) {}
                    continue;
                }
                
                if (!check(TokenType::IDENTIFIER)) {
                    error("Expected field name");
                    return nullptr;
                }
                const Token& field_name = peek();
                std::string fname = field_name.value;
                advance();
                
                // Handle bitfield syntax: int x : N
                if (match(TokenType::COLON)) {
                    // Skip the bit width
                    if (check(TokenType::INTEGER)) advance();
                }
                
                auto field = std::make_unique<StructFieldNode>(
                    field_type, fname, field_name.line, field_name.column);
                struct_decl->fields.push_back(std::move(field));
                
                expect(TokenType::SEMICOLON);
            }
            expect(TokenType::RBRACE);
            expect(TokenType::SEMICOLON);
            return std::move(struct_decl);
        } else {
            // This is a variable/param declaration using struct type: struct Name varname;
            std::string full_type = "struct " + struct_name;
            if (check(TokenType::IDENTIFIER)) {
                const Token& name_token = peek();
                std::string var_name = name_token.value;
                advance();
                
                if (check(TokenType::LPAREN)) {
                    return parse_function_decl(full_type);
                } else {
                    return parse_var_decl(full_type);
                }
            }
            error("Expected identifier after struct name");
            return nullptr;
        }
    }
    
    // Handle union keyword (treat as struct for now)
    if (match(TokenType::KW_UNION)) {
        if (!check(TokenType::IDENTIFIER)) {
            error("Expected union name");
            return nullptr;
        }
        std::string union_name = peek().value;
        advance();
        
        if (check(TokenType::LBRACE)) {
            auto union_decl = std::make_unique<StructDeclNode>(
                union_name, tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
            
            expect(TokenType::LBRACE);
            while (!check(TokenType::RBRACE) && !is_at_end()) {
                std::string field_type = parse_type_specifier();
                if (!check(TokenType::IDENTIFIER)) {
                    error("Expected field name");
                    return nullptr;
                }
                const Token& field_name = peek();
                std::string fname = field_name.value;
                advance();
                
                auto field = std::make_unique<StructFieldNode>(
                    field_type, fname, field_name.line, field_name.column);
                union_decl->fields.push_back(std::move(field));
                
                expect(TokenType::SEMICOLON);
            }
            expect(TokenType::RBRACE);
            expect(TokenType::SEMICOLON);
            return std::move(union_decl);
        } else {
            std::string full_type = "union " + union_name;
            if (check(TokenType::IDENTIFIER)) {
                const Token& name_token = peek();
                std::string var_name = name_token.value;
                advance();
                return parse_var_decl(full_type);
            }
            error("Expected identifier after union name");
            return nullptr;
        }
    }
    
    // Handle enum keyword
    if (match(TokenType::KW_ENUM)) {
        return parse_enum_decl();
    }
    
    // Handle typedef keyword
    if (match(TokenType::KW_TYPEDEF)) {
        return parse_typedef_decl();
    }
    
    if (!is_type_specifier()) {
        error("Expected type specifier");
        return nullptr;
    }
    
    std::string type_name = parse_type_specifier();
    
    // Handle function pointer declarations: int (*fp)(int, int)
    if (check(TokenType::LPAREN)) {
        size_t saved_pos = pos_;
        advance(); // consume (
        if (check(TokenType::STAR)) {
            advance(); // consume *
            if (check(TokenType::IDENTIFIER)) {
                std::string ptr_name = peek().value;
                advance(); // consume name
                if (match(TokenType::RPAREN)) {
                    // We have int (*fp) - now check for (params)
                    if (match(TokenType::LPAREN)) {
                        // Skip parameters
                        int depth = 1;
                        while (depth > 0 && !is_at_end()) {
                            if (check(TokenType::LPAREN)) depth++;
                            if (check(TokenType::RPAREN)) depth--;
                            advance();
                        }
                        // Now parse as variable declaration with function pointer type
                        auto var = std::make_unique<VarDeclNode>(
                            type_name + "(*)()", ptr_name, tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
                        if (match(TokenType::ASSIGN)) {
                            var->initializer = parse_expression();
                        }
                        expect(TokenType::SEMICOLON);
                        return std::move(var);
                    }
                }
            }
        }
        // Not a function pointer, restore and try normal path
        pos_ = saved_pos;
    }
    
    if (!check(TokenType::IDENTIFIER)) {
        error("Expected identifier after type");
        return nullptr;
    }
    
    const Token& name_token = peek();
    std::string name = name_token.value;
    advance();
    
    if (check(TokenType::LPAREN)) {
        return parse_function_decl(type_name);
    } else {
        return parse_var_decl(type_name);
    }
}

ASTPtr Parser::parse_function_decl(const std::string& type_name) {
    const Token& name_token = tokens_[pos_ - 1];
    auto func = std::make_unique<FunctionDeclNode>(
        type_name, name_token.value, name_token.line, name_token.column);
    
    expect(TokenType::LPAREN);
    
    // Parse parameters
    if (!check(TokenType::RPAREN)) {
        do {
            auto param = parse_param();
            if (param) {
                func->params.push_back(std::move(param));
            }
        } while (match(TokenType::COMMA));
    }
    
    expect(TokenType::RPAREN);
    
    // Forward declaration: no body
    if (check(TokenType::SEMICOLON)) {
        advance(); // consume semicolon
        func->body = nullptr;
    } else {
        func->body = parse_block();
    }
    
    return std::move(func);
}

ASTPtr Parser::parse_var_decl(const std::string& type_name) {
    const Token& name_token = tokens_[pos_ - 1];
    auto var = std::make_unique<VarDeclNode>(
        type_name, name_token.value, name_token.line, name_token.column);
    
    // Skip __attribute__((...)) if present
    while (check(TokenType::KW_ATTRIBUTE)) {
        advance(); // consume __attribute__
        if (match(TokenType::LPAREN)) {
            if (match(TokenType::LPAREN)) {
                int depth = 1;
                while (depth > 0 && !is_at_end()) {
                    if (check(TokenType::LPAREN)) depth++;
                    if (check(TokenType::RPAREN)) depth--;
                    if (depth > 0) advance();
                }
                if (!is_at_end()) advance(); // consume ))
            }
            if (!is_at_end()) advance(); // consume )
        }
    }
    
    // Check for array declaration
    if (match(TokenType::LBRACKET)) {
        if (check(TokenType::INTEGER)) {
            var->array_size = std::stoi(peek().value);
            advance();
        }
        expect(TokenType::RBRACKET);
    }
    
    if (match(TokenType::ASSIGN)) {
        // Handle braced initializer: { expr, expr, ... }
        if (check(TokenType::LBRACE)) {
            advance(); // consume {
            int depth = 1;
            while (depth > 0 && !is_at_end()) {
                if (check(TokenType::LBRACE)) depth++;
                if (check(TokenType::RBRACE) && depth > 0) depth--;
                if (depth > 0) advance();
            }
            if (!is_at_end()) advance(); // consume closing }
            // Create a dummy integer 0 as initializer
            var->initializer = std::make_unique<IntegerLiteralNode>(0, var->line, var->column);
        } else {
            var->initializer = parse_expression();
        }
    }
    
    expect(TokenType::SEMICOLON);
    return std::move(var);
}

ASTPtr Parser::parse_struct_decl() {
    const Token& name_token = peek();
    std::string name = name_token.value;
    advance();
    
    auto struct_decl = std::make_unique<StructDeclNode>(
        name, name_token.line, name_token.column);
    
    expect(TokenType::LBRACE);
    
    // Parse fields
    while (!check(TokenType::RBRACE) && !is_at_end()) {
        std::string field_type = parse_type_specifier();
        if (!check(TokenType::IDENTIFIER)) {
            error("Expected field name");
            return nullptr;
        }
        const Token& field_name = peek();
        std::string fname = field_name.value;
        advance();
        
        auto field = std::make_unique<StructFieldNode>(
            field_type, fname, field_name.line, field_name.column);
        struct_decl->fields.push_back(std::move(field));
        
        expect(TokenType::SEMICOLON);
    }
    
    expect(TokenType::RBRACE);
    expect(TokenType::SEMICOLON);
    
    return std::move(struct_decl);
}

ASTPtr Parser::parse_enum_decl() {
    const Token& name_token = peek();
    std::string name = name_token.value;
    advance();
    
    auto enum_decl = std::make_unique<EnumDeclNode>(
        name, name_token.line, name_token.column);
    
    expect(TokenType::LBRACE);
    
    long long value = 0;
    while (!check(TokenType::RBRACE) && !is_at_end()) {
        const Token& val_token = peek();
        std::string val_name = val_token.value;
        advance();
        
        ASTPtr val_expr = nullptr;
        if (match(TokenType::ASSIGN)) {
            val_expr = parse_assignment();
            if (val_expr && val_expr->type == NodeType::INTEGER_LITERAL) {
                value = static_cast<IntegerLiteralNode*>(val_expr.get())->value;
            }
        }
        
        // Store enum constant for later resolution
        enum_constants_[val_name] = value;
        
        auto enum_val = std::make_unique<EnumValueNode>(
            val_name, std::move(val_expr), val_token.line, val_token.column);
        enum_decl->values.push_back(std::move(enum_val));
        
        match(TokenType::COMMA); // optional trailing comma
        value++;
    }
    
    expect(TokenType::RBRACE);
    expect(TokenType::SEMICOLON);
    
    return std::move(enum_decl);
}

ASTPtr Parser::parse_typedef_decl() {
    std::string source_type = parse_type_specifier();
    
    if (!check(TokenType::IDENTIFIER)) {
        error("Expected typedef name");
        return nullptr;
    }
    
    const Token& alias_token = peek();
    std::string alias = alias_token.value;
    advance();
    
    // Register typedef name for type specifier recognition
    typedef_names_.insert(alias);
    
    expect(TokenType::SEMICOLON);
    
    return std::make_unique<TypedefDeclNode>(
        source_type, alias, alias_token.line, alias_token.column);
}

ASTPtr Parser::parse_param() {
    // Handle variadic parameter ...
    if (match(TokenType::ELLIPSIS)) {
        auto param = std::make_unique<ParamNode>(
            "...", "...", tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
        return std::move(param);
    }
    
    std::string type_name = parse_type_specifier();
    
    if (!check(TokenType::IDENTIFIER)) {
        // Could be array parameter like int arr[10] or just type with no name
        if (check(TokenType::LBRACKET)) {
            // Array parameter - consume [size] and treat as pointer
            advance(); // consume [
            if (check(TokenType::INTEGER)) advance(); // consume size
            expect(TokenType::RBRACKET);
            // Now expect the parameter name
        }
        if (!check(TokenType::IDENTIFIER)) {
            error("Expected parameter name");
            return nullptr;
        }
    }
    
    const Token& name_token = peek();
    auto param = std::make_unique<ParamNode>(
        type_name, name_token.value, name_token.line, name_token.column);
    advance();
    
    // Handle array parameter syntax: name[size]
    if (match(TokenType::LBRACKET)) {
        if (check(TokenType::INTEGER)) advance(); // consume size
        expect(TokenType::RBRACKET);
        param->type_name += "*"; // decay to pointer
    }
    
    return std::move(param);
}

ASTPtr Parser::parse_block() {
    expect(TokenType::LBRACE);
    
    auto block = std::make_unique<BlockNode>(tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
    
    while (!check(TokenType::RBRACE) && !is_at_end()) {
        auto stmt = parse_statement();
        if (stmt) {
            block->statements.push_back(std::move(stmt));
        }
    }
    
    expect(TokenType::RBRACE);
    return std::move(block);
}

ASTPtr Parser::parse_statement() {
    if (check(TokenType::KW_RETURN)) {
        return parse_return_stmt();
    }
    if (check(TokenType::KW_IF)) {
        return parse_if_stmt();
    }
    if (check(TokenType::KW_WHILE)) {
        return parse_while_stmt();
    }
    if (check(TokenType::KW_DO)) {
        return parse_do_while_stmt();
    }
    if (check(TokenType::KW_FOR)) {
        return parse_for_stmt();
    }
    if (check(TokenType::KW_SWITCH)) {
        return parse_switch_stmt();
    }
    if (check(TokenType::KW_GOTO)) {
        return parse_goto_stmt();
    }
    if (check(TokenType::KW_BREAK)) {
        advance();
        expect(TokenType::SEMICOLON);
        return std::make_unique<BreakStmtNode>(tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
    }
    if (check(TokenType::KW_CONTINUE)) {
        advance();
        expect(TokenType::SEMICOLON);
        return std::make_unique<ContinueStmtNode>(tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
    }
    if (check(TokenType::KW_STATIC_ASSERT)) {
        return parse_static_assert();
    }
    if (check(TokenType::LBRACE)) {
        return parse_block();
    }
    if (is_type_specifier()) {
        std::string type_name = parse_type_specifier();
        // Check for function pointer: int (*fp)(int, int)
        if (check(TokenType::LPAREN)) {
            size_t saved_pos = pos_;
            advance(); // consume (
            if (check(TokenType::STAR)) {
                advance(); // consume *
                if (check(TokenType::IDENTIFIER)) {
                    std::string ptr_name = peek().value;
                    advance(); // consume name
                    if (match(TokenType::RPAREN)) {
                        if (match(TokenType::LPAREN)) {
                            // Skip parameter types
                            int depth = 1;
                            while (depth > 0 && !is_at_end()) {
                                if (check(TokenType::LPAREN)) depth++;
                                else if (check(TokenType::RPAREN)) {
                                    depth--;
                                    if (depth == 0) { advance(); break; }
                                }
                                advance();
                            }
                            auto var = std::make_unique<VarDeclNode>(
                                type_name + "(*)()", ptr_name, tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
                            if (match(TokenType::ASSIGN)) {
                                var->initializer = parse_expression();
                            }
                            expect(TokenType::SEMICOLON);
                            return std::move(var);
                        }
                    }
                }
            }
            pos_ = saved_pos;
        }
        if (!check(TokenType::IDENTIFIER)) {
            error("Expected identifier after type");
            return nullptr;
        }
        advance(); // consume identifier
        // Check if this is a function declaration (nested function)
        if (check(TokenType::LPAREN)) {
            return parse_function_decl(type_name);
        }
        return parse_var_decl(type_name);
    }
    // Label statement: identifier COLON
    if (check(TokenType::IDENTIFIER) && peek_next().type == TokenType::COLON) {
        std::string label_name = peek().value;
        advance(); // consume identifier
        advance(); // consume colon
        auto stmt = parse_statement();
        return std::make_unique<LabelStmtNode>(label_name, tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
    }
    return parse_expr_stmt();
}

ASTPtr Parser::parse_return_stmt() {
    const Token& return_token = peek();
    advance();
    
    auto ret = std::make_unique<ReturnStmtNode>(return_token.line, return_token.column);
    
    if (!check(TokenType::SEMICOLON)) {
        ret->value = parse_expression();
    }
    
    expect(TokenType::SEMICOLON);
    return std::move(ret);
}

ASTPtr Parser::parse_expr_stmt() {
    const Token& expr_token = peek();
    auto expr = parse_expression();
    
    auto stmt = std::make_unique<ExprStmtNode>(expr_token.line, expr_token.column);
    stmt->expr = std::move(expr);
    
    expect(TokenType::SEMICOLON);
    return std::move(stmt);
}

ASTPtr Parser::parse_if_stmt() {
    const Token& if_token = peek();
    advance();
    
    auto if_stmt = std::make_unique<IfStmtNode>(if_token.line, if_token.column);
    
    expect(TokenType::LPAREN);
    if_stmt->condition = parse_expression();
    expect(TokenType::RPAREN);
    
    if_stmt->then_branch = parse_statement();
    
    if (match(TokenType::KW_ELSE)) {
        if_stmt->else_branch = parse_statement();
    }
    
    return std::move(if_stmt);
}

ASTPtr Parser::parse_while_stmt() {
    const Token& while_token = peek();
    advance();
    
    auto while_stmt = std::make_unique<WhileStmtNode>(while_token.line, while_token.column);
    
    expect(TokenType::LPAREN);
    while_stmt->condition = parse_expression();
    expect(TokenType::RPAREN);
    
    while_stmt->body = parse_statement();
    
    return std::move(while_stmt);
}

ASTPtr Parser::parse_do_while_stmt() {
    const Token& do_token = peek();
    advance();
    
    auto do_while_stmt = std::make_unique<DoWhileStmtNode>(do_token.line, do_token.column);
    
    do_while_stmt->body = parse_statement();
    
    expect(TokenType::KW_WHILE);
    expect(TokenType::LPAREN);
    do_while_stmt->condition = parse_expression();
    expect(TokenType::RPAREN);
    expect(TokenType::SEMICOLON);
    
    return std::move(do_while_stmt);
}

ASTPtr Parser::parse_for_stmt() {
    const Token& for_token = peek();
    advance();
    
    auto for_stmt = std::make_unique<ForStmtNode>(for_token.line, for_token.column);
    
    expect(TokenType::LPAREN);
    
    // Init
    if (is_type_specifier()) {
        std::string type = parse_type_specifier();
        for_stmt->init = parse_var_decl(type);
    } else if (!check(TokenType::SEMICOLON)) {
        for_stmt->init = parse_expr_stmt();
    } else {
        advance(); // skip semicolon
    }
    
    // Condition
    if (!check(TokenType::SEMICOLON)) {
        for_stmt->condition = parse_expression();
    }
    expect(TokenType::SEMICOLON);
    
    // Update
    if (!check(TokenType::RPAREN)) {
        for_stmt->update = parse_expression();
    }
    expect(TokenType::RPAREN);
    
    for_stmt->body = parse_statement();
    
    return std::move(for_stmt);
}

ASTPtr Parser::parse_switch_stmt() {
    const Token& switch_token = peek();
    advance();
    
    auto switch_stmt = std::make_unique<SwitchStmtNode>(switch_token.line, switch_token.column);
    
    expect(TokenType::LPAREN);
    switch_stmt->condition = parse_expression();
    expect(TokenType::RPAREN);
    
    // Parse body - cases are labels within the body
    expect(TokenType::LBRACE);
    while (!check(TokenType::RBRACE) && !is_at_end()) {
        if (check(TokenType::KW_CASE)) {
            advance();
            auto case_label = std::make_unique<CaseLabelNode>(tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
            case_label->value = parse_expression();
            expect(TokenType::COLON);
            while (!check(TokenType::KW_CASE) && !check(TokenType::KW_DEFAULT) && !check(TokenType::RBRACE) && !is_at_end()) {
                case_label->body = parse_statement();
            }
            switch_stmt->cases.push_back(std::move(case_label));
        } else if (check(TokenType::KW_DEFAULT)) {
            advance();
            auto default_label = std::make_unique<DefaultLabelNode>(tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
            expect(TokenType::COLON);
            while (!check(TokenType::KW_CASE) && !check(TokenType::KW_DEFAULT) && !check(TokenType::RBRACE) && !is_at_end()) {
                default_label->body = parse_statement();
            }
            switch_stmt->cases.push_back(std::move(default_label));
        } else {
            auto stmt = parse_statement();
            if (stmt && switch_stmt->cases.empty()) {
                auto default_label = std::make_unique<DefaultLabelNode>(stmt->line, stmt->column);
                default_label->body = std::move(stmt);
                switch_stmt->cases.push_back(std::move(default_label));
            }
        }
    }
    expect(TokenType::RBRACE);
    
    return std::move(switch_stmt);
}

ASTPtr Parser::parse_goto_stmt() {
    const Token& goto_token = peek();
    advance();
    
    // Indirect goto: goto *expr;
    if (match(TokenType::STAR)) {
        auto expr = parse_expression();
        expect(TokenType::SEMICOLON);
        // For now, treat indirect goto as a no-op (would need computed goto support)
        return std::make_unique<ExprStmtNode>(goto_token.line, goto_token.column);
    }
    
    if (!check(TokenType::IDENTIFIER)) {
        error("Expected label name after goto");
        return nullptr;
    }
    
    std::string label = peek().value;
    advance();
    expect(TokenType::SEMICOLON);
    
    return std::make_unique<GotoStmtNode>(label, goto_token.line, goto_token.column);
}

ASTPtr Parser::parse_static_assert() {
    int line = peek().line;
    int col = peek().column;
    advance(); // consume _Static_assert
    expect(TokenType::LPAREN);
    auto condition = parse_expression();
    // Skip the message string
    if (match(TokenType::COMMA)) {
        if (check(TokenType::STRING_LITERAL)) advance();
    }
    expect(TokenType::RPAREN);
    expect(TokenType::SEMICOLON);
    // Static assertions are checked at compile time; for now skip them
    // by returning an empty expression statement
    return std::make_unique<ExprStmtNode>(line, col);
}

// Expression parsing with precedence climbing

ASTPtr Parser::parse_expression() {
    auto left = parse_assignment();
    
    // Comma operator
    while (match(TokenType::COMMA)) {
        auto comma = std::make_unique<CommaExprNode>(
            left->line, left->column);
        comma->left = std::move(left);
        comma->right = parse_assignment();
        left = std::move(comma);
    }
    
    return std::move(left);
}

ASTPtr Parser::parse_assignment() {
    auto left = parse_or();
    
    // Ternary operator
    if (match(TokenType::QUESTION)) {
        auto ternary = std::make_unique<TernaryExprNode>(
            left->line, left->column);
        ternary->condition = std::move(left);
        ternary->then_expr = parse_expression();
        expect(TokenType::COLON);
        ternary->else_expr = parse_assignment();
        return std::move(ternary);
    }
    
    if (match(TokenType::ASSIGN)) {
        auto assign = std::make_unique<AssignExprNode>(
            left->line, left->column);
        assign->target = std::move(left);
        assign->value = parse_assignment();
        return std::move(assign);
    }
    
    // Compound assignment operators
    if (check(TokenType::PLUS_ASSIGN) || check(TokenType::MINUS_ASSIGN) ||
        check(TokenType::STAR_ASSIGN) || check(TokenType::SLASH_ASSIGN)) {
        
        OpKind op;
        if (match(TokenType::PLUS_ASSIGN)) op = OpKind::ADD;
        else if (match(TokenType::MINUS_ASSIGN)) op = OpKind::SUB;
        else if (match(TokenType::STAR_ASSIGN)) op = OpKind::MUL;
        else { match(TokenType::SLASH_ASSIGN); op = OpKind::DIV; }
        
        auto compound = std::make_unique<CompoundAssignExprNode>(
            op, left->line, left->column);
        compound->target = std::move(left);
        compound->value = parse_assignment();
        return std::move(compound);
    }
    
    return std::move(left);
}

ASTPtr Parser::parse_or() {
    auto left = parse_and();
    
    while (match(TokenType::OR)) {
        auto bin = std::make_unique<BinaryExprNode>(OpKind::OR, left->line, left->column);
        bin->left = std::move(left);
        bin->right = parse_and();
        left = std::move(bin);
    }
    
    return std::move(left);
}

ASTPtr Parser::parse_and() {
    auto left = parse_bitwise_or();
    
    while (match(TokenType::AND)) {
        auto bin = std::make_unique<BinaryExprNode>(OpKind::AND, left->line, left->column);
        bin->left = std::move(left);
        bin->right = parse_bitwise_or();
        left = std::move(bin);
    }
    
    return std::move(left);
}

ASTPtr Parser::parse_bitwise_or() {
    auto left = parse_bitwise_xor();
    
    while (match(TokenType::PIPE)) {
        auto bin = std::make_unique<BinaryExprNode>(OpKind::BIT_OR, left->line, left->column);
        bin->left = std::move(left);
        bin->right = parse_bitwise_xor();
        left = std::move(bin);
    }
    
    return std::move(left);
}

ASTPtr Parser::parse_bitwise_xor() {
    auto left = parse_bitwise_and();
    
    while (match(TokenType::CARET)) {
        auto bin = std::make_unique<BinaryExprNode>(OpKind::BIT_XOR, left->line, left->column);
        bin->left = std::move(left);
        bin->right = parse_bitwise_and();
        left = std::move(bin);
    }
    
    return std::move(left);
}

ASTPtr Parser::parse_bitwise_and() {
    auto left = parse_equality();
    
    while (match(TokenType::AMPERSAND)) {
        auto bin = std::make_unique<BinaryExprNode>(OpKind::BIT_AND, left->line, left->column);
        bin->left = std::move(left);
        bin->right = parse_equality();
        left = std::move(bin);
    }
    
    return std::move(left);
}

ASTPtr Parser::parse_equality() {
    auto left = parse_comparison();
    
    while (check(TokenType::EQ) || check(TokenType::NE)) {
        OpKind op = match(TokenType::EQ) ? OpKind::EQ : OpKind::NE;
        auto bin = std::make_unique<BinaryExprNode>(op, left->line, left->column);
        bin->left = std::move(left);
        bin->right = parse_comparison();
        left = std::move(bin);
    }
    
    return std::move(left);
}

ASTPtr Parser::parse_comparison() {
    auto left = parse_shift();
    
    while (check(TokenType::LT) || check(TokenType::GT) || 
           check(TokenType::LE) || check(TokenType::GE)) {
        OpKind op;
        if (match(TokenType::LT)) op = OpKind::LT;
        else if (match(TokenType::GT)) op = OpKind::GT;
        else if (match(TokenType::LE)) op = OpKind::LE;
        else op = OpKind::GE;
        
        auto bin = std::make_unique<BinaryExprNode>(op, left->line, left->column);
        bin->left = std::move(left);
        bin->right = parse_shift();
        left = std::move(bin);
    }
    
    return std::move(left);
}

ASTPtr Parser::parse_shift() {
    auto left = parse_addition();
    
    while (check(TokenType::LSHIFT) || check(TokenType::RSHIFT)) {
        OpKind op = match(TokenType::LSHIFT) ? OpKind::LSHIFT : OpKind::RSHIFT;
        auto bin = std::make_unique<BinaryExprNode>(op, left->line, left->column);
        bin->left = std::move(left);
        bin->right = parse_addition();
        left = std::move(bin);
    }
    
    return std::move(left);
}

ASTPtr Parser::parse_addition() {
    auto left = parse_multiplication();
    
    while (check(TokenType::PLUS) || check(TokenType::MINUS)) {
        OpKind op;
        if (match(TokenType::PLUS)) {
            op = OpKind::ADD;
        } else {
            match(TokenType::MINUS);
            op = OpKind::SUB;
        }
        auto bin = std::make_unique<BinaryExprNode>(op, left->line, left->column);
        bin->left = std::move(left);
        bin->right = parse_multiplication();
        left = std::move(bin);
    }
    
    return std::move(left);
}

ASTPtr Parser::parse_multiplication() {
    auto left = parse_unary();
    
    while (check(TokenType::STAR) || check(TokenType::SLASH) || check(TokenType::PERCENT)) {
        OpKind op;
        if (match(TokenType::STAR)) op = OpKind::MUL;
        else if (match(TokenType::SLASH)) op = OpKind::DIV;
        else op = OpKind::MOD;
        
        auto bin = std::make_unique<BinaryExprNode>(op, left->line, left->column);
        bin->left = std::move(left);
        bin->right = parse_unary();
        left = std::move(bin);
    }
    
    return std::move(left);
}

ASTPtr Parser::parse_unary() {
    // sizeof operator
    if (match(TokenType::KW_SIZEOF)) {
        int line = tokens_[pos_ - 1].line;
        int col = tokens_[pos_ - 1].column;
        
        if (match(TokenType::LPAREN)) {
            // sizeof(type)
            if (is_type_specifier()) {
                std::string type = parse_type_specifier();
                expect(TokenType::RPAREN);
                return std::make_unique<SizeofExprNode>(type, line, col);
            } else {
                // sizeof(expr)
                auto expr = parse_expression();
                expect(TokenType::RPAREN);
                return std::make_unique<SizeofExprNode>(std::move(expr), line, col);
            }
        } else {
            // sizeof expr
            auto expr = parse_unary();
            return std::make_unique<SizeofExprNode>(std::move(expr), line, col);
        }
    }
    
    // _Alignof operator (same as sizeof for alignment purposes)
    if (match(TokenType::KW_ALIGNOF)) {
        int line = tokens_[pos_ - 1].line;
        int col = tokens_[pos_ - 1].column;
        
        if (match(TokenType::LPAREN)) {
            if (is_type_specifier()) {
                std::string type = parse_type_specifier();
                expect(TokenType::RPAREN);
                return std::make_unique<SizeofExprNode>(type, line, col);
            } else {
                auto expr = parse_expression();
                expect(TokenType::RPAREN);
                return std::make_unique<SizeofExprNode>(std::move(expr), line, col);
            }
        } else {
            auto expr = parse_unary();
            return std::make_unique<SizeofExprNode>(std::move(expr), line, col);
        }
    }
    
    // _Generic selection: _Generic(control-expr, type1: expr1, ..., default: expr)
    if (match(TokenType::KW_GENERIC)) {
        int line = tokens_[pos_ - 1].line;
        int col = tokens_[pos_ - 1].column;
        
        expect(TokenType::LPAREN);
        auto control = parse_assignment();
        expect(TokenType::COMMA);
        
        // Parse the first association, then look for default
        ASTPtr result = nullptr;
        while (!check(TokenType::RPAREN) && !is_at_end()) {
            if (match(TokenType::KW_DEFAULT)) {
                expect(TokenType::COLON);
                result = parse_assignment();
            } else {
                // Skip type name
                parse_type_specifier();
                expect(TokenType::COLON);
                auto assoc_expr = parse_assignment();
                if (!result) result = std::move(assoc_expr);
            }
            if (!match(TokenType::COMMA)) break;
        }
        expect(TokenType::RPAREN);
        
        // Simplified: just return the first/default expression
        return result ? std::move(result) : std::make_unique<IntegerLiteralNode>(0, line, col);
    }
    
    // Type cast: (type)expr - peek ahead to check if LPAREN is followed by a type
    if (check(TokenType::LPAREN)) {
        size_t saved_pos = pos_;
        advance(); // peek past (
        if (is_type_specifier()) {
            int line = tokens_[pos_ - 1].line;
            int col = tokens_[pos_ - 1].column;
            std::string type = parse_type_specifier();
            if (match(TokenType::RPAREN)) {
                auto expr = parse_unary();
                return std::make_unique<CastExprNode>(type, std::move(expr), line, col);
            }
        }
        // Not a cast, restore position
        pos_ = saved_pos;
    }
    
    if (match(TokenType::MINUS)) {
        auto unary = std::make_unique<UnaryExprNode>(OpKind::UMINUS, tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
        unary->operand = parse_unary();
        return std::move(unary);
    }
    if (match(TokenType::PLUS)) {
        auto unary = std::make_unique<UnaryExprNode>(OpKind::UPLUS, tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
        unary->operand = parse_unary();
        return std::move(unary);
    }
    if (match(TokenType::NOT)) {
        auto unary = std::make_unique<UnaryExprNode>(OpKind::NOT, tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
        unary->operand = parse_unary();
        return std::move(unary);
    }
    if (match(TokenType::STAR)) {
        auto unary = std::make_unique<UnaryExprNode>(OpKind::DEREF, tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
        unary->operand = parse_unary();
        return std::move(unary);
    }
    if (match(TokenType::AMPERSAND)) {
        auto unary = std::make_unique<UnaryExprNode>(OpKind::ADDRESS_OF, tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
        unary->operand = parse_unary();
        return std::move(unary);
    }
    // GCC label address-of: &&label
    if (check(TokenType::AND) && peek_next().type == TokenType::IDENTIFIER) {
        advance(); // consume &&
        std::string label = peek().value;
        advance(); // consume label name
        // Return as an integer literal placeholder (actual label address resolved at link time)
        return std::make_unique<IntegerLiteralNode>(0, tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
    }
    if (match(TokenType::PLUS_PLUS)) {
        auto unary = std::make_unique<UnaryExprNode>(OpKind::PRE_INC, tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
        unary->operand = parse_unary();
        return std::move(unary);
    }
    if (match(TokenType::MINUS_MINUS)) {
        auto unary = std::make_unique<UnaryExprNode>(OpKind::PRE_DEC, tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
        unary->operand = parse_unary();
        return std::move(unary);
    }
    
    return parse_postfix();
}

ASTPtr Parser::parse_postfix() {
    auto primary = parse_primary();
    
    if (!primary) return nullptr;
    
    while (true) {
        if (match(TokenType::LPAREN)) {
            // Function call
            auto call = std::make_unique<CallExprNode>(
                static_cast<IdentifierExprNode*>(primary.get())->name,
                primary->line, primary->column);
            
            if (!check(TokenType::RPAREN)) {
                do {
                    call->arguments.push_back(parse_assignment());
                } while (match(TokenType::COMMA));
            }
            
            expect(TokenType::RPAREN);
            primary = std::move(call);
        } else if (match(TokenType::LBRACKET)) {
            // Array index
            auto index = std::make_unique<IndexExprNode>(primary->line, primary->column);
            index->array = std::move(primary);
            index->index = parse_expression();
            expect(TokenType::RBRACKET);
            primary = std::move(index);
        } else if (match(TokenType::DOT)) {
            // Member access
            auto member = std::make_unique<MemberExprNode>(false, primary->line, primary->column);
            member->object = std::move(primary);
            member->member = peek().value;
            expect(TokenType::IDENTIFIER);
            primary = std::move(member);
        } else if (match(TokenType::MINUS_GT)) {
            // Arrow member access
            auto member = std::make_unique<MemberExprNode>(true, primary->line, primary->column);
            member->object = std::move(primary);
            member->member = peek().value;
            expect(TokenType::IDENTIFIER);
            primary = std::move(member);
        } else if (match(TokenType::PLUS_PLUS)) {
            auto unary = std::make_unique<UnaryExprNode>(OpKind::POST_INC, primary->line, primary->column);
            unary->operand = std::move(primary);
            primary = std::move(unary);
        } else if (match(TokenType::MINUS_MINUS)) {
            auto unary = std::make_unique<UnaryExprNode>(OpKind::POST_DEC, primary->line, primary->column);
            unary->operand = std::move(primary);
            primary = std::move(unary);
        } else {
            break;
        }
    }
    
    return std::move(primary);
}

ASTPtr Parser::parse_primary() {
    if (match(TokenType::INTEGER)) {
        const Token& tok = tokens_[pos_ - 1];
        long long value = std::stoll(tok.value);
        return std::make_unique<IntegerLiteralNode>(value, tok.line, tok.column);
    }
    
    if (match(TokenType::FLOAT)) {
        const Token& tok = tokens_[pos_ - 1];
        double value = std::stod(tok.value);
        return std::make_unique<FloatLiteralNode>(value, tok.line, tok.column);
    }
    
    if (match(TokenType::STRING_LITERAL)) {
        const Token& tok = tokens_[pos_ - 1];
        return std::make_unique<StringLiteralNode>(tok.value, tok.line, tok.column);
    }
    
    if (match(TokenType::CHAR_LITERAL)) {
        const Token& tok = tokens_[pos_ - 1];
        char value = tok.value.empty() ? '\0' : tok.value[0];
        return std::make_unique<CharLiteralNode>(value, tok.line, tok.column);
    }
    
    if (match(TokenType::IDENTIFIER)) {
        const Token& tok = tokens_[pos_ - 1];
        // Check if this is an enum constant
        if (enum_constants_.count(tok.value)) {
            return std::make_unique<IntegerLiteralNode>(enum_constants_[tok.value], tok.line, tok.column);
        }
        return std::make_unique<IdentifierExprNode>(tok.value, tok.line, tok.column);
    }
    
    if (match(TokenType::LPAREN)) {
        // Check for statement expression: ({ ... })
        if (check(TokenType::LBRACE)) {
            int line = tokens_[pos_ - 1].line;
            int col = tokens_[pos_ - 1].column;
            auto block = parse_block();
            expect(TokenType::RPAREN);
            return std::make_unique<StmtExprNode>(line, col);
        }
        auto expr = parse_expression();
        expect(TokenType::RPAREN);
        return std::move(expr);
    }
    
    error("Unexpected token in expression");
    return nullptr;
}

} // namespace simplecc
