#include <catch2/catch_test_macros.hpp>
#include "parser.h"
#include "lexer.h"

using namespace simplecc;

ProgramNode* parse_source(const std::string& source) {
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    REQUIRE_FALSE(lexer.has_error());
    
    Parser parser(tokens);
    auto ast = parser.parse();
    REQUIRE_FALSE(parser.has_error());
    
    return static_cast<ProgramNode*>(ast.release());
}

TEST_CASE("Parse empty program", "[parser]") {
    auto program = std::unique_ptr<ProgramNode>(parse_source(""));
    REQUIRE(program->type == NodeType::PROGRAM);
    REQUIRE(program->declarations.empty());
}

TEST_CASE("Parse integer literal", "[parser]") {
    auto program = std::unique_ptr<ProgramNode>(parse_source("42;"));
    REQUIRE(program->type == NodeType::PROGRAM);
    REQUIRE(program->declarations.size() == 1);
    
    auto& stmt = static_cast<ExprStmtNode&>(*program->declarations[0]);
    REQUIRE(stmt.type == NodeType::EXPR_STMT);
    
    auto& lit = static_cast<IntegerLiteralNode&>(*stmt.expr);
    REQUIRE(lit.type == NodeType::INTEGER_LITERAL);
    REQUIRE(lit.value == 42);
}

TEST_CASE("Parse binary expression", "[parser]") {
    auto program = std::unique_ptr<ProgramNode>(parse_source("2 + 3;"));
    REQUIRE(program->declarations.size() == 1);
    
    auto& stmt = static_cast<ExprStmtNode&>(*program->declarations[0]);
    auto& bin = static_cast<BinaryExprNode&>(*stmt.expr);
    REQUIRE(bin.type == NodeType::BINARY_EXPR);
    REQUIRE(bin.op == OpKind::ADD);
    
    auto& left = static_cast<IntegerLiteralNode&>(*bin.left);
    REQUIRE(left.value == 2);
    
    auto& right = static_cast<IntegerLiteralNode&>(*bin.right);
    REQUIRE(right.value == 3);
}

TEST_CASE("Parse operator precedence", "[parser]") {
    auto program = std::unique_ptr<ProgramNode>(parse_source("2 + 3 * 4;"));
    
    auto& stmt = static_cast<ExprStmtNode&>(*program->declarations[0]);
    auto& bin = static_cast<BinaryExprNode&>(*stmt.expr);
    REQUIRE(bin.op == OpKind::ADD);
    
    auto& right = static_cast<BinaryExprNode&>(*bin.right);
    REQUIRE(right.op == OpKind::MUL);
}

TEST_CASE("Parse variable declaration", "[parser]") {
    auto program = std::unique_ptr<ProgramNode>(parse_source("int x = 42;"));
    REQUIRE(program->declarations.size() == 1);
    
    auto& var = static_cast<VarDeclNode&>(*program->declarations[0]);
    REQUIRE(var.type == NodeType::VAR_DECL);
    REQUIRE(var.type_name == "int");
    REQUIRE(var.name == "x");
    REQUIRE(var.initializer != nullptr);
    
    auto& init = static_cast<IntegerLiteralNode&>(*var.initializer);
    REQUIRE(init.value == 42);
}

TEST_CASE("Parse function declaration", "[parser]") {
    auto program = std::unique_ptr<ProgramNode>(parse_source("int main() { return 0; }"));
    REQUIRE(program->declarations.size() == 1);
    
    auto& func = static_cast<FunctionDeclNode&>(*program->declarations[0]);
    REQUIRE(func.type == NodeType::FUNCTION_DECL);
    REQUIRE(func.return_type == "int");
    REQUIRE(func.name == "main");
    REQUIRE(func.params.empty());
    REQUIRE(func.body != nullptr);
}

TEST_CASE("Parse function with parameters", "[parser]") {
    auto program = std::unique_ptr<ProgramNode>(parse_source("int add(int a, int b) { return a + b; }"));
    
    auto& func = static_cast<FunctionDeclNode&>(*program->declarations[0]);
    REQUIRE(func.params.size() == 2);
    
    auto& param1 = static_cast<ParamNode&>(*func.params[0]);
    REQUIRE(param1.type_name == "int");
    REQUIRE(param1.name == "a");
    
    auto& param2 = static_cast<ParamNode&>(*func.params[1]);
    REQUIRE(param2.type_name == "int");
    REQUIRE(param2.name == "b");
}

TEST_CASE("Parse return statement", "[parser]") {
    auto program = std::unique_ptr<ProgramNode>(parse_source("int main() { return 42; }"));
    
    auto& func = static_cast<FunctionDeclNode&>(*program->declarations[0]);
    auto& body = static_cast<BlockNode&>(*func.body);
    REQUIRE(body.statements.size() == 1);
    
    auto& ret = static_cast<ReturnStmtNode&>(*body.statements[0]);
    REQUIRE(ret.type == NodeType::RETURN_STMT);
    REQUIRE(ret.value != nullptr);
    
    auto& value = static_cast<IntegerLiteralNode&>(*ret.value);
    REQUIRE(value.value == 42);
}

TEST_CASE("Parse if statement", "[parser]") {
    auto program = std::unique_ptr<ProgramNode>(parse_source("int main() { if (1) return 1; }"));
    
    auto& func = static_cast<FunctionDeclNode&>(*program->declarations[0]);
    auto& body = static_cast<BlockNode&>(*func.body);
    auto& if_stmt = static_cast<IfStmtNode&>(*body.statements[0]);
    
    REQUIRE(if_stmt.type == NodeType::IF_STMT);
    REQUIRE(if_stmt.condition != nullptr);
    REQUIRE(if_stmt.then_branch != nullptr);
    REQUIRE(if_stmt.else_branch == nullptr);
}

TEST_CASE("Parse if-else statement", "[parser]") {
    auto program = std::unique_ptr<ProgramNode>(parse_source("int main() { if (1) return 1; else return 2; }"));
    
    auto& func = static_cast<FunctionDeclNode&>(*program->declarations[0]);
    auto& body = static_cast<BlockNode&>(*func.body);
    auto& if_stmt = static_cast<IfStmtNode&>(*body.statements[0]);
    
    REQUIRE(if_stmt.else_branch != nullptr);
}

TEST_CASE("Parse while statement", "[parser]") {
    auto program = std::unique_ptr<ProgramNode>(parse_source("int main() { while (1) return 1; }"));
    
    auto& func = static_cast<FunctionDeclNode&>(*program->declarations[0]);
    auto& body = static_cast<BlockNode&>(*func.body);
    auto& while_stmt = static_cast<WhileStmtNode&>(*body.statements[0]);
    
    REQUIRE(while_stmt.type == NodeType::WHILE_STMT);
    REQUIRE(while_stmt.condition != nullptr);
    REQUIRE(while_stmt.body != nullptr);
}

TEST_CASE("Parse for statement", "[parser]") {
    auto program = std::unique_ptr<ProgramNode>(parse_source("int main() { for (;;) return 1; }"));
    
    auto& func = static_cast<FunctionDeclNode&>(*program->declarations[0]);
    auto& body = static_cast<BlockNode&>(*func.body);
    auto& for_stmt = static_cast<ForStmtNode&>(*body.statements[0]);
    
    REQUIRE(for_stmt.type == NodeType::FOR_STMT);
    REQUIRE(for_stmt.body != nullptr);
}

TEST_CASE("Parse unary operators", "[parser]") {
    SECTION("Unary minus") {
        auto program = std::unique_ptr<ProgramNode>(parse_source("-5;"));
        auto& stmt = static_cast<ExprStmtNode&>(*program->declarations[0]);
        auto& unary = static_cast<UnaryExprNode&>(*stmt.expr);
        REQUIRE(unary.op == OpKind::UMINUS);
    }
    
    SECTION("Logical not") {
        auto program = std::unique_ptr<ProgramNode>(parse_source("!1;"));
        auto& stmt = static_cast<ExprStmtNode&>(*program->declarations[0]);
        auto& unary = static_cast<UnaryExprNode&>(*stmt.expr);
        REQUIRE(unary.op == OpKind::NOT);
    }
}

TEST_CASE("Parse comparison operators", "[parser]") {
    auto program = std::unique_ptr<ProgramNode>(parse_source("1 < 2;"));
    auto& stmt = static_cast<ExprStmtNode&>(*program->declarations[0]);
    auto& bin = static_cast<BinaryExprNode&>(*stmt.expr);
    REQUIRE(bin.op == OpKind::LT);
}

TEST_CASE("Parse assignment", "[parser]") {
    auto program = std::unique_ptr<ProgramNode>(parse_source("int x; x = 5;"));
    REQUIRE(program->declarations.size() == 2);
    
    auto& assign_stmt = static_cast<ExprStmtNode&>(*program->declarations[1]);
    auto& assign = static_cast<AssignExprNode&>(*assign_stmt.expr);
    REQUIRE(assign.type == NodeType::ASSIGN_EXPR);
    REQUIRE(assign.target != nullptr);
    REQUIRE(assign.value != nullptr);
}

TEST_CASE("Parse complex expression", "[parser]") {
    auto program = std::unique_ptr<ProgramNode>(parse_source("(2 + 3) * 4;"));
    
    auto& stmt = static_cast<ExprStmtNode&>(*program->declarations[0]);
    auto& mul = static_cast<BinaryExprNode&>(*stmt.expr);
    REQUIRE(mul.op == OpKind::MUL);
    
    auto& add = static_cast<BinaryExprNode&>(*mul.left);
    REQUIRE(add.op == OpKind::ADD);
}

TEST_CASE("Parse multiple declarations", "[parser]") {
    auto program = std::unique_ptr<ProgramNode>(parse_source("int x = 1; int y = 2; int z = 3;"));
    REQUIRE(program->declarations.size() == 3);
}

TEST_CASE("Parse nested blocks", "[parser]") {
    auto program = std::unique_ptr<ProgramNode>(parse_source("int main() { { { return 1; } } }"));
    
    auto& func = static_cast<FunctionDeclNode&>(*program->declarations[0]);
    auto& body = static_cast<BlockNode&>(*func.body);
    REQUIRE(body.statements.size() == 1);
    
    auto& inner_block1 = static_cast<BlockNode&>(*body.statements[0]);
    REQUIRE(inner_block1.statements.size() == 1);
    
    auto& inner_block2 = static_cast<BlockNode&>(*inner_block1.statements[0]);
    REQUIRE(inner_block2.statements.size() == 1);
}

TEST_CASE("Parse error: missing semicolon", "[parser]") {
    Lexer lexer("int x = 42");
    auto tokens = lexer.tokenize();
    
    Parser parser(tokens);
    auto ast = parser.parse();
    REQUIRE(parser.has_error());
}

TEST_CASE("Parse error: missing closing paren", "[parser]") {
    Lexer lexer("int main( { return 0; }");
    auto tokens = lexer.tokenize();
    
    Parser parser(tokens);
    auto ast = parser.parse();
    REQUIRE(parser.has_error());
}
