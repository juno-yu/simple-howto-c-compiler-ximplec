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

// Skip C23 attribute [[...]] — supports multiple in a row, including [[name(args)]].
void Parser::skip_c23_attributes() {
    while (check(TokenType::LBRACKET) && pos_ + 1 < tokens_.size() &&
           tokens_[pos_ + 1].type == TokenType::LBRACKET) {
        advance(); // consume first [
        advance(); // consume second [
        int depth = 1;
        while (depth > 0 && !is_at_end()) {
            if (check(TokenType::LBRACKET) && pos_ + 1 < tokens_.size() &&
                tokens_[pos_ + 1].type == TokenType::LBRACKET) {
                advance();
                advance();
                depth++;
                continue;
            }
            if (check(TokenType::RBRACKET) && pos_ + 1 < tokens_.size() &&
                tokens_[pos_ + 1].type == TokenType::RBRACKET) {
                advance();
                advance();
                depth--;
                continue;
            }
            // Handle balanced parens inside (e.g. [[nodiscard("msg")]]).
            if (check(TokenType::LPAREN)) {
                int pdepth = 1;
                advance();
                while (pdepth > 0 && !is_at_end()) {
                    if (check(TokenType::LPAREN)) pdepth++;
                    if (check(TokenType::RPAREN)) pdepth--;
                    advance();
                }
                continue;
            }
            // Handle strings inside (e.g. [[nodiscard("msg")]]).
            if (check(TokenType::STRING_LITERAL)) {
                advance();
                continue;
            }
            advance();
        }
    }
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
        check(TokenType::KW_ATOMIC) ||
        check(TokenType::KW_CONSTEXPR)) {
        return true;
    }
    // Check if current identifier is a typedef name or size_t
    if (check(TokenType::IDENTIFIER)) {
        const std::string& name = peek().value;
        if (name == "size_t" || name == "ssize_t" || name == "ptrdiff_t" ||
            name == "intptr_t" || name == "uintptr_t" ||
            name == "uint8_t" || name == "uint16_t" || name == "uint32_t" || name == "uint64_t" ||
            name == "int8_t" || name == "int16_t" || name == "int32_t" || name == "int64_t") {
            return true;
        }
        return typedef_names_.count(name);
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
        } else if (match(TokenType::KW_INLINE)) {
            result += "inline ";
        } else if (match(TokenType::KW_REGISTER)) {
            result += "register ";
        } else if (match(TokenType::KW_AUTO)) {
            result += "auto ";
        } else if (match(TokenType::KW_RESTRICT)) {
            result += "restrict ";
        } else if (match(TokenType::KW_CONSTEXPR)) {
            // constexpr is a no-op for codegen purposes (no constant eval yet)
            result += "constexpr ";
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
        if (check(TokenType::IDENTIFIER)) {
            result += "enum " + peek().value;
            advance();
        } else {
            // Anonymous enum: enum {A, B, C};
            expect(TokenType::LBRACE);
            long long val = 0;
            while (!check(TokenType::RBRACE) && !is_at_end()) {
                std::string cname = peek().value;
                advance();
                if (match(TokenType::ASSIGN)) {
                    auto expr = parse_assignment();
                    if (expr && expr->type == NodeType::INTEGER_LITERAL) {
                        val = static_cast<IntegerLiteralNode*>(expr.get())->value;
                    }
                }
                enum_constants_[cname] = val;
                match(TokenType::COMMA);
                val++;
            }
            expect(TokenType::RBRACE);
            result += "int";
        }
    } else if (match(TokenType::KW_TYPEDEF)) {
        // typedef name used as type - just return the name
        if (!check(TokenType::IDENTIFIER)) {
            error("Expected typedef name");
            return result;
        }
        result += peek().value;
        advance();
    } else if (result.empty() || is_unsigned || is_signed || long_count || short_count) {
        // Check if this is a typedef name or built-in type alias
        if (check(TokenType::IDENTIFIER)) {
            const std::string& name = peek().value;
            if (name == "size_t" || name == "ssize_t" || name == "ptrdiff_t" ||
                name == "intptr_t" || name == "uintptr_t" ||
                name == "uint8_t" || name == "uint16_t" || name == "uint32_t" || name == "uint64_t" ||
                name == "int8_t" || name == "int16_t" || name == "int32_t" || name == "int64_t") {
                result += name;
                advance();
            } else if (typedef_names_.count(name)) {
                result += name;
                advance();
            } else if (result.empty()) {
                error("Expected type specifier");
            }
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

    while (!is_at_end() && !has_error()) {
        // Skip C23 attributes at top level
        skip_c23_attributes();
        ASTPtr decl;
        if (is_type_specifier() || check(TokenType::LBRACKET)) {
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
        std::string type_name = parse_type_specifier();
        if (!check(TokenType::IDENTIFIER)) {
            error("Expected identifier after extern");
            return nullptr;
        }
        const Token& name_token = peek();
        std::string name = name_token.value;
        
        // Check if function or variable before advancing
        size_t saved_pos = pos_;
        advance(); // consume name
        
        if (check(TokenType::LPAREN)) {
            // Function declaration
            auto func = std::make_unique<FunctionDeclNode>(
                type_name, name, name_token.line, name_token.column);
            advance(); // consume (
            if (!check(TokenType::RPAREN)) {
                do {
                    auto param = parse_param();
                    if (param) func->params.push_back(std::move(param));
                } while (match(TokenType::COMMA));
            }
            expect(TokenType::RPAREN);
            expect(TokenType::SEMICOLON);
            func->body = nullptr;
            return std::move(func);
        } else {
            // Variable declaration — delegate to parse_var_decl for multi-declarator support
            pos_ = saved_pos; // back before the identifier name
            advance();        // consume the identifier name
            return parse_var_decl(type_name, true);
        }
    }
    
    // Handle struct keyword
    if (match(TokenType::KW_STRUCT)) {
        // Skip C23 attributes between struct and name (e.g. struct [[nodiscard]] S)
        skip_c23_attributes();
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
                
                // Handle flexible array member: int data[]
                if (check(TokenType::LBRACKET)) {
                    advance(); // consume [
                    if (check(TokenType::RBRACKET)) {
                        advance(); // consume ] — flexible array
                        field_type += "[]";
                    } else {
                        // Regular array field
                        if (check(TokenType::INTEGER)) advance();
                        expect(TokenType::RBRACKET);
                    }
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
            // Allow pointer types: struct Name *p, struct Name **pp
            while (check(TokenType::STAR)) {
                full_type += " *";
                advance();
            }
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
            while (check(TokenType::STAR)) {
                full_type += " *";
                advance();
            }
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
        // Skip C23 attributes between enum and name (e.g. enum [[nodiscard]] S)
        skip_c23_attributes();
        // Check for anonymous enum: enum { A, B, C };
        if (check(TokenType::LBRACE)) {
            expect(TokenType::LBRACE);
            long long val = 0;
            while (!check(TokenType::RBRACE) && !is_at_end()) {
                std::string cname = peek().value;
                advance();
                if (match(TokenType::ASSIGN)) {
                    auto expr = parse_assignment();
                    if (expr && expr->type == NodeType::INTEGER_LITERAL) {
                        val = static_cast<IntegerLiteralNode*>(expr.get())->value;
                    }
                }
                enum_constants_[cname] = val;
                match(TokenType::COMMA);
                val++;
            }
            expect(TokenType::RBRACE);
            match(TokenType::SEMICOLON);
            return std::make_unique<ExprStmtNode>(
                tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
        }
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

    // A function is nested if we are currently inside another function's body.
    bool is_nested = !function_stack_.empty();
    func->is_nested = is_nested;
    if (is_nested) {
        func->parent_function = function_stack_.back();
    }

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
        function_stack_.push_back(func->name);
        func->body = parse_block();
        function_stack_.pop_back();
    }

    return std::move(func);
}

ASTPtr Parser::parse_var_decl(const std::string& type_name, bool is_extern) {
    const Token& name_token = tokens_[pos_ - 1];
    auto var = std::make_unique<VarDeclNode>(
        type_name, name_token.value, name_token.line, name_token.column);
    var->is_extern = is_extern;
    
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
    
    // Check for array declaration (supports multi-dim: int arr[2][3][4])
    if (match(TokenType::LBRACKET)) {
        std::vector<int> dims;
        // First dimension
        if (check(TokenType::INTEGER)) {
            dims.push_back(std::stoi(peek().value));
            var->array_size = dims[0];
            advance();
        } else {
            dims.push_back(0);
        }
        expect(TokenType::RBRACKET);
        // Additional dimensions: collect all, collapse total size
        while (check(TokenType::LBRACKET)) {
            advance();
            int dim_size = 0;
            if (check(TokenType::INTEGER)) {
                dim_size = std::stoi(peek().value);
                dims.push_back(dim_size);
                advance();
            } else {
                dims.push_back(0);
            }
            expect(TokenType::RBRACKET);
            if (dim_size > 0) {
                var->array_size = (var->array_size > 0 ? var->array_size : 1) * dim_size;
            }
        }
        if (dims.size() > 1) {
            multidim_dims_[var->name] = dims;
        }
    }
    
    if (match(TokenType::ASSIGN)) {
        if (check(TokenType::LBRACE)) {
            var->initializer = parse_brace_initializer();
        } else {
            var->initializer = parse_assignment();
        }
    }

    // Handle multiple declarators: int a, b, c; or char *p, *q;
    if (check(TokenType::COMMA)) {
        auto block = std::make_unique<BlockNode>(var->line, var->column);
        block->statements.push_back(std::move(var));
        while (match(TokenType::COMMA)) {
            // Handle pointer prefix: *q in "char *p, *q"
            while (match(TokenType::STAR)) {
                // Skip qualifiers after * (e.g., const, volatile, restrict)
                while (check(TokenType::KW_CONST) || check(TokenType::KW_VOLATILE) ||
                       check(TokenType::KW_RESTRICT) || check(TokenType::KW_ATOMIC)) {
                    advance();
                }
            }
            if (!check(TokenType::IDENTIFIER)) break;
            const Token& next_name = peek();
            auto next_var = std::make_unique<VarDeclNode>(
                type_name, next_name.value, next_name.line, next_name.column);
            next_var->is_extern = is_extern;
            advance();
            // Array (multi-dim)
            if (match(TokenType::LBRACKET)) {
                std::vector<int> ndims;
                if (check(TokenType::INTEGER)) {
                    ndims.push_back(std::stoi(peek().value));
                    next_var->array_size = ndims[0];
                    advance();
                } else {
                    ndims.push_back(0);
                }
                expect(TokenType::RBRACKET);
                while (check(TokenType::LBRACKET)) {
                    advance();
                    int dim_size = 0;
                    if (check(TokenType::INTEGER)) {
                        dim_size = std::stoi(peek().value);
                        ndims.push_back(dim_size);
                        advance();
                    } else {
                        ndims.push_back(0);
                    }
                    expect(TokenType::RBRACKET);
                    if (dim_size > 0) {
                        next_var->array_size = (next_var->array_size > 0 ? next_var->array_size : 1) * dim_size;
                    }
                }
                if (ndims.size() > 1) {
                    multidim_dims_[next_var->name] = ndims;
                }
            }
            // Initializer
            if (match(TokenType::ASSIGN)) {
                if (check(TokenType::LBRACE)) {
                    next_var->initializer = parse_brace_initializer();
                } else {
                    next_var->initializer = parse_assignment();
                }
            }
            block->statements.push_back(std::move(next_var));
        }
        expect(TokenType::SEMICOLON);
        return std::move(block);
    }

    expect(TokenType::SEMICOLON);
    return std::move(var);
}

ASTPtr Parser::parse_brace_initializer() {
    int line = peek().line;
    int col = peek().column;
    expect(TokenType::LBRACE);
    auto list = std::make_unique<InitializerListNode>(line, col);
    if (!check(TokenType::RBRACE)) {
        do {
            ASTPtr elem;
            if (check(TokenType::DOT)) {
                // .field = expr
                advance();
                if (!check(TokenType::IDENTIFIER)) {
                    error("Expected field name after '.'");
                    return nullptr;
                }
                std::string field = peek().value;
                advance();
                if (!match(TokenType::ASSIGN)) {
                    error("Expected '=' after field name");
                    return nullptr;
                }
                // Use parse_assignment so comma operator doesn't grab the list separator
                auto val = parse_assignment();
                if (!val) return nullptr;
                elem = std::make_unique<DesignatedInitNode>(field, std::move(val), line, col);
            } else if (check(TokenType::LBRACKET)) {
                // [index] = expr
                advance();
                if (!check(TokenType::INTEGER)) {
                    error("Expected integer index");
                    return nullptr;
                }
                int idx = std::stoi(peek().value);
                advance();
                if (!match(TokenType::RBRACKET)) {
                    error("Expected ']'");
                    return nullptr;
                }
                if (!match(TokenType::ASSIGN)) {
                    error("Expected '=' after index");
                    return nullptr;
                }
                auto val = parse_assignment();
                if (!val) return nullptr;
                elem = std::make_unique<DesignatedInitNode>(idx, std::move(val), line, col);
            } else {
                // Plain positional initializer: use parse_assignment to stop at comma
                elem = parse_assignment();
                if (!elem) return nullptr;
            }
            list->elements.push_back(std::move(elem));
        } while (match(TokenType::COMMA));
    }
    if (!expect(TokenType::RBRACE)) {
        return nullptr;
    }
    return std::move(list);
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
    std::string name;
    int name_line = peek().line, name_col = peek().column;
    if (check(TokenType::IDENTIFIER)) {
        name = peek().value;
        advance();
    }
    
    auto enum_decl = std::make_unique<EnumDeclNode>(
        name, name_line, name_col);
    
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
    
    std::string alias;
    int alias_line, alias_col;
    
    if (check(TokenType::IDENTIFIER)) {
        alias_line = peek().line;
        alias_col = peek().column;
        alias = peek().value;
        advance();
    } else if (match(TokenType::LPAREN)) {
        // Function pointer typedef: typedef int (*func_t)(int);
        if (match(TokenType::STAR)) {
            if (!check(TokenType::IDENTIFIER)) {
                error("Expected function pointer name");
                return nullptr;
            }
            alias_line = peek().line;
            alias_col = peek().column;
            alias = peek().value;
            advance();
            expect(TokenType::RPAREN);
        } else {
            error("Expected typedef name");
            return nullptr;
        }
        // Skip the parameter list
        if (match(TokenType::LPAREN)) {
            int depth = 1;
            while (depth > 0 && !is_at_end()) {
                if (check(TokenType::LPAREN)) depth++;
                else if (check(TokenType::RPAREN)) {
                    depth--;
                    if (depth == 0) { advance(); break; }
                }
                advance();
            }
        }
        source_type = source_type + "(*)";
    } else {
        error("Expected typedef name");
        return nullptr;
    }
    
    typedef_names_.insert(alias);
    expect(TokenType::SEMICOLON);
    
    return std::make_unique<TypedefDeclNode>(
        source_type, alias, alias_line, alias_col);
}

ASTPtr Parser::parse_param() {
    // Handle variadic parameter ...
    if (match(TokenType::ELLIPSIS)) {
        auto param = std::make_unique<ParamNode>(
            "...", "...", tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
        return std::move(param);
    }
    
    std::string type_name = parse_type_specifier();

    // C2x syntax: `int f(void)` — void as the sole parameter means "no parameters".
    if (type_name == "void" && check(TokenType::RPAREN)) {
        // No parameters; return a dummy param (the caller will check for empty).
        return std::make_unique<ParamNode>("void", "", tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
    }

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
    
    while (!check(TokenType::RBRACE) && !is_at_end() && !has_error()) {
        size_t prev_pos = pos_;
        skip_c23_attributes();
        auto stmt = parse_statement();
        if (stmt) {
            block->statements.push_back(std::move(stmt));
        }
        // Recover from errors: if we didn't advance, skip a token to avoid
        // an infinite loop when parse_statement returns without consuming input.
        if (pos_ == prev_pos) {
            advance();
        }
    }

    expect(TokenType::RBRACE);
    return std::move(block);
}

ASTPtr Parser::parse_statement() {
    // Skip C23 attributes at the start of a statement (e.g. `if (x) [[likely]] {...}`).
    skip_c23_attributes();
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
    // GCC inline assembly: asm("...") or asm volatile("...")
    if (check(TokenType::IDENTIFIER) && peek().value == "asm") {
        advance(); // consume 'asm'
        if (check(TokenType::IDENTIFIER) && peek().value == "volatile") {
            advance(); // consume 'volatile' (optional)
        }
        if (!match(TokenType::LPAREN)) {
            error("Expected ( after asm");
            return nullptr;
        }
        if (!check(TokenType::STRING_LITERAL)) {
            error("Expected assembly string literal");
            return nullptr;
        }
        std::string asm_str = peek().value;
        advance(); // consume string
        std::string output_ops, input_ops, clobber_regs;
        // Parse extended asm: asm("..." : out : in : clobber)
        if (match(TokenType::COLON)) {
            // Output operands
            if (!check(TokenType::COLON) && !check(TokenType::RPAREN)) {
                output_ops = parse_asm_operands();
            }
            if (match(TokenType::COLON)) {
                // Input operands
                if (!check(TokenType::COLON) && !check(TokenType::RPAREN)) {
                    input_ops = parse_asm_operands();
                }
                if (match(TokenType::COLON)) {
                    // Clobber list
                    if (!check(TokenType::RPAREN)) {
                        clobber_regs = parse_asm_clobbers();
                    }
                }
            }
        }
        expect(TokenType::RPAREN);
        expect(TokenType::SEMICOLON);
        auto node = std::make_unique<AsmStmtNode>(asm_str, tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
        node->output = output_ops;
        node->input = input_ops;
        node->clobber = clobber_regs;
        return std::move(node);
    }
    if (check(TokenType::KW_STATIC_ASSERT)) {
        return parse_static_assert();
    }
    if (check(TokenType::KW_TYPEDEF)) {
        advance();
        std::string source_type = parse_type_specifier();
        if (check(TokenType::IDENTIFIER)) {
            std::string alias = peek().value;
            advance();
            typedef_names_.insert(alias);
            expect(TokenType::SEMICOLON);
            return std::make_unique<TypedefDeclNode>(
                source_type, alias, tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
        } else if (check(TokenType::LPAREN)) {
            advance();
            expect(TokenType::STAR);
            if (!check(TokenType::IDENTIFIER)) {
                error("Expected function pointer name");
                return nullptr;
            }
            std::string alias = peek().value;
            advance();
            expect(TokenType::RPAREN);
            expect(TokenType::LPAREN);
            int depth = 1;
            while (depth > 0 && !is_at_end()) {
                if (check(TokenType::LPAREN)) depth++;
                else if (check(TokenType::RPAREN)) {
                    depth--;
                    if (depth == 0) { advance(); break; }
                }
                advance();
            }
            typedef_names_.insert(alias);
            expect(TokenType::SEMICOLON);
            return std::make_unique<TypedefDeclNode>(
                source_type + "(*)", alias, tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
        }
        error("Expected typedef name");
        return nullptr;
    }
    if (check(TokenType::LBRACE)) {
        return parse_block();
    }
    if (is_type_specifier()) {
        std::string type_name = parse_type_specifier();
        if (check(TokenType::SEMICOLON)) {
            advance();
            return std::make_unique<ExprStmtNode>(
                tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
        }
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
    // Handle empty statement (standalone semicolon)
    if (check(TokenType::SEMICOLON)) {
        int line = peek().line, col = peek().column;
        advance();
        return std::make_unique<ExprStmtNode>(line, col);
    }
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
        if (check(TokenType::IDENTIFIER)) {
            advance(); // consume identifier — parse_var_decl expects pos past name
        }
        for_stmt->init = parse_var_decl(type);
        // parse_var_decl stops at `;` only via the for-loop's semicolon below
    } else if (!check(TokenType::SEMICOLON)) {
        for_stmt->init = parse_expression();
        expect(TokenType::SEMICOLON);
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
                size_t prev_pos = pos_;
                skip_c23_attributes();
                case_label->body = parse_statement();
                if (pos_ == prev_pos) advance();
            }
            switch_stmt->cases.push_back(std::move(case_label));
        } else if (check(TokenType::KW_DEFAULT)) {
            advance();
            auto default_label = std::make_unique<DefaultLabelNode>(tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
            expect(TokenType::COLON);
            while (!check(TokenType::KW_CASE) && !check(TokenType::KW_DEFAULT) && !check(TokenType::RBRACE) && !is_at_end()) {
                size_t prev_pos = pos_;
                skip_c23_attributes();
                default_label->body = parse_statement();
                if (pos_ == prev_pos) advance();
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

std::string Parser::parse_asm_operands() {
    // Parse comma-separated operands: "constraint" (expr), ...
    std::string result;
    while (!check(TokenType::COLON) && !check(TokenType::RPAREN) && !is_at_end()) {
        if (!result.empty()) result += ", ";
        // Skip the operand string and expression
        if (check(TokenType::STRING_LITERAL)) advance();
        if (match(TokenType::LPAREN)) {
            int depth = 1;
            while (depth > 0 && !is_at_end()) {
                if (check(TokenType::LPAREN)) depth++;
                if (check(TokenType::RPAREN)) depth--;
                if (depth > 0) advance();
            }
        }
    }
    return result;
}

std::string Parser::parse_asm_clobbers() {
    // Parse comma-separated clobber strings: "eax", "memory", ...
    std::string result;
    while (!check(TokenType::RPAREN) && !is_at_end()) {
        if (!result.empty()) result += ", ";
        if (check(TokenType::STRING_LITERAL)) {
            result += peek().value;
            advance();
        }
        match(TokenType::COMMA);
    }
    return result;
}

// Expression parsing with precedence climbing

ASTPtr Parser::parse_expression() {
    auto left = parse_assignment();
    if (!left) return nullptr;

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
        check(TokenType::STAR_ASSIGN) || check(TokenType::SLASH_ASSIGN) ||
        check(TokenType::PERCENT_ASSIGN) || check(TokenType::AMP_ASSIGN) ||
        check(TokenType::PIPE_ASSIGN) || check(TokenType::CARET_ASSIGN) ||
        check(TokenType::LSHIFT_ASSIGN) || check(TokenType::RSHIFT_ASSIGN)) {

        OpKind op;
        if (match(TokenType::PLUS_ASSIGN)) op = OpKind::ADD;
        else if (match(TokenType::MINUS_ASSIGN)) op = OpKind::SUB;
        else if (match(TokenType::STAR_ASSIGN)) op = OpKind::MUL;
        else if (match(TokenType::SLASH_ASSIGN)) op = OpKind::DIV;
        else if (match(TokenType::PERCENT_ASSIGN)) op = OpKind::MOD;
        else if (match(TokenType::AMP_ASSIGN)) op = OpKind::BIT_AND;
        else if (match(TokenType::PIPE_ASSIGN)) op = OpKind::BIT_OR;
        else if (match(TokenType::CARET_ASSIGN)) op = OpKind::BIT_XOR;
        else if (match(TokenType::LSHIFT_ASSIGN)) op = OpKind::LSHIFT;
        else { match(TokenType::RSHIFT_ASSIGN); op = OpKind::RSHIFT; }

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
        OpKind op;
        if (match(TokenType::EQ)) op = OpKind::EQ;
        else { match(TokenType::NE); op = OpKind::NE; }
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
        else { match(TokenType::GE); op = OpKind::GE; }
        
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
        OpKind op;
        if (match(TokenType::LSHIFT)) op = OpKind::LSHIFT;
        else { match(TokenType::RSHIFT); op = OpKind::RSHIFT; }
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
        else { match(TokenType::PERCENT); op = OpKind::MOD; }
        
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
            // Handle array declarator in compound literal: (int[N]){...}
            // Empty brackets mean deduce size from initializer.
            if (match(TokenType::LBRACKET)) {
                if (check(TokenType::INTEGER)) {
                    type += "[" + peek().value + "]";
                    advance();
                } else {
                    // Empty brackets - mark with -1 sentinel
                    type += "[-1]";
                }
                expect(TokenType::RBRACKET);
            }
            if (match(TokenType::RPAREN)) {
                // Check for compound literal: (type){...}
                if (check(TokenType::LBRACE)) {
                    auto init = parse_brace_initializer();
                    if (!init) return nullptr;
                    // If brackets were empty, deduce size from initializer
                    if (type.size() >= 4 && type.substr(type.size() - 4) == "[-1]") {
                        int count = 0;
                        if (auto* list = dynamic_cast<InitializerListNode*>(init.get())) {
                            for (auto& e : list->elements) {
                                if (dynamic_cast<DesignatedInitNode*>(e.get())) continue;
                                count++;
                            }
                        }
                        type = type.substr(0, type.size() - 4) + "[" + std::to_string(count) + "]";
                    }
                    return std::make_unique<CompoundLiteralNode>(type, std::move(init), line, col);
                }
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
    if (match(TokenType::TILDE)) {
        auto unary = std::make_unique<UnaryExprNode>(OpKind::BIT_NOT, tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
        unary->operand = parse_unary();
        return std::move(unary);
    }
    
    return parse_postfix();
}

ASTPtr Parser::parse_postfix() {
    auto primary = parse_primary();

    if (!primary) return nullptr;

    // Helper: get full dimension vector for a multi-dim array variable
    auto get_dims = [&](const std::string& name) -> std::vector<int>* {
        auto it = multidim_dims_.find(name);
        if (it != multidim_dims_.end()) return &it->second;
        return nullptr;
    };

    // Helper: get the variable name from an expression (for multi-dim lookup)
    auto get_array_name = [&](ASTNode* node) -> std::string {
        if (auto* id = dynamic_cast<IdentifierExprNode*>(node))
            return id->name;
        return "";
    };

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
            auto first_index = parse_expression();
            expect(TokenType::RBRACKET);

            // Collect all remaining [indices] for N-dim flattening
            std::vector<ASTPtr> indices;
            indices.push_back(std::move(first_index));
            while (check(TokenType::LBRACKET)) {
                advance();
                indices.push_back(parse_expression());
                expect(TokenType::RBRACKET);
            }

            // Try to flatten N dimensions
            std::string arr_name = get_array_name(primary.get());
            std::vector<int>* dims = arr_name.empty() ? nullptr : get_dims(arr_name);

            if (dims && indices.size() > 1) {
                // N-dim flatten: compute flat index from dims and indices
                // For int a[M][N][P]: strides = {N*P, P, 1}
                int line = primary->line, col = primary->column;
                int ndims = dims->size();

                // Compute strides: strides[i] = product of dims[i+1..end]
                std::vector<int> strides(ndims, 1);
                for (int i = ndims - 2; i >= 0; i--) {
                    strides[i] = strides[i + 1] * (*dims)[i + 1];
                }

                // Build flat index expression: sum of indices[i] * strides[i]
                // Start with first index * first stride
                ASTPtr flat;
                {
                    auto* first_idx = indices[0].get();
                    int s = (0 < ndims) ? strides[0] : 1;
                    if (s != 1) {
                        auto mul = std::make_unique<BinaryExprNode>(OpKind::MUL, line, col);
                        mul->left = std::move(indices[0]);
                        mul->right = std::make_unique<IntegerLiteralNode>(s, line, col);
                        flat = std::move(mul);
                    } else {
                        flat = std::move(indices[0]);
                    }
                }

                // Add remaining: flat = flat + indices[i] * strides[i]
                for (size_t i = 1; i < indices.size(); i++) {
                    int s = (i < (size_t)ndims) ? strides[i] : 1;
                    ASTPtr term;
                    if (s != 1) {
                        auto mul = std::make_unique<BinaryExprNode>(OpKind::MUL, line, col);
                        mul->left = std::move(indices[i]);
                        mul->right = std::make_unique<IntegerLiteralNode>(s, line, col);
                        term = std::move(mul);
                    } else {
                        term = std::move(indices[i]);
                    }
                    auto add = std::make_unique<BinaryExprNode>(OpKind::ADD, line, col);
                    add->left = std::move(flat);
                    add->right = std::move(term);
                    flat = std::move(add);
                }

                // Wrap in single IndexExprNode for lvalue
                auto result = std::make_unique<IndexExprNode>(line, col);
                result->array = std::move(primary);
                result->index = std::move(flat);
                primary = std::move(result);
            } else {
                // Single-dim or non-identifier: just nest
                ASTPtr result = std::move(primary);
                for (auto& idx : indices) {
                    auto node = std::make_unique<IndexExprNode>(result->line, result->column);
                    node->array = std::move(result);
                    node->index = std::move(idx);
                    result = std::move(node);
                }
                primary = std::move(result);
            }
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
        long long value = 0;
        const std::string& s = tok.value;
        if (s.size() > 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
            value = std::stoll(s.substr(2), nullptr, 16);
        } else if (s.size() > 2 && s[0] == '0' && (s[1] == 'b' || s[1] == 'B')) {
            value = std::stoll(s.substr(2), nullptr, 2);
        } else if (s.size() > 1 && s[0] == '0') {
            value = std::stoll(s, nullptr, 8);
        } else {
            value = std::stoll(s);
        }
        return std::make_unique<IntegerLiteralNode>(value, tok.line, tok.column);
    }
    
    if (match(TokenType::FLOAT)) {
        const Token& tok = tokens_[pos_ - 1];
        std::string v = tok.value;
        bool is_float_type = (!v.empty() && v[0] == 'F');
        if (is_float_type) v = v.substr(1);
        double value = std::stod(v);
        auto node = std::make_unique<FloatLiteralNode>(value, tok.line, tok.column);
        node->is_single_precision = is_float_type;
        return std::move(node);
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
            auto stmt_expr = std::make_unique<StmtExprNode>(line, col);
            stmt_expr->body = std::move(block);
            return std::move(stmt_expr);
        }
        auto expr = parse_expression();
        expect(TokenType::RPAREN);
        return std::move(expr);
    }
    
    // Nested brace initializer: { expr, expr, ... }
    if (check(TokenType::LBRACE)) {
        return parse_brace_initializer();
    }
    
    error("Unexpected token in expression");
    return nullptr;
}

} // namespace simplecc
