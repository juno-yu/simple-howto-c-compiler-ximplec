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
    return check(TokenType::KW_INT) || 
           check(TokenType::KW_CHAR) || 
           check(TokenType::KW_VOID) ||
           check(TokenType::KW_BOOL);
}

std::string Parser::parse_type_specifier() {
    if (match(TokenType::KW_INT)) return "int";
    if (match(TokenType::KW_CHAR)) return "char";
    if (match(TokenType::KW_VOID)) return "void";
    if (match(TokenType::KW_BOOL)) return "bool";
    error("Expected type specifier");
    return "";
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
    if (!is_type_specifier()) {
        error("Expected type specifier");
        return nullptr;
    }
    
    std::string type_name = parse_type_specifier();
    
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
    func->body = parse_block();
    
    return std::move(func);
}

ASTPtr Parser::parse_var_decl(const std::string& type_name) {
    const Token& name_token = tokens_[pos_ - 1];
    auto var = std::make_unique<VarDeclNode>(
        type_name, name_token.value, name_token.line, name_token.column);
    
    if (match(TokenType::ASSIGN)) {
        var->initializer = parse_expression();
    }
    
    expect(TokenType::SEMICOLON);
    return std::move(var);
}

ASTPtr Parser::parse_param() {
    std::string type_name = parse_type_specifier();
    
    if (!check(TokenType::IDENTIFIER)) {
        error("Expected parameter name");
        return nullptr;
    }
    
    const Token& name_token = peek();
    auto param = std::make_unique<ParamNode>(
        type_name, name_token.value, name_token.line, name_token.column);
    advance();
    
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
    if (check(TokenType::LBRACE)) {
        return parse_block();
    }
    if (is_type_specifier()) {
        std::string type_name = parse_type_specifier();
        if (!check(TokenType::IDENTIFIER)) {
            error("Expected identifier after type");
            return nullptr;
        }
        advance(); // consume identifier
        return parse_var_decl(type_name);
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
        else op = OpKind::DIV;
        
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
    
    while (true) {
        if (match(TokenType::LPAREN)) {
            // Function call
            auto call = std::make_unique<CallExprNode>(
                static_cast<IdentifierExprNode*>(primary.get())->name,
                primary->line, primary->column);
            
            if (!check(TokenType::RPAREN)) {
                do {
                    call->arguments.push_back(parse_expression());
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
        return std::make_unique<IdentifierExprNode>(tok.value, tok.line, tok.column);
    }
    
    if (match(TokenType::LPAREN)) {
        auto expr = parse_expression();
        expect(TokenType::RPAREN);
        return std::move(expr);
    }
    
    error("Unexpected token in expression");
    return nullptr;
}

} // namespace simplecc
