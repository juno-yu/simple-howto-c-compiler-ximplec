#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
#include "lexer.h"
#include "parser.h"
#include "codegen.h"

using namespace simplecc;

TEST_CASE("Full pipeline: simple return", "[integration]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { return 42; }");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $42, %rax") != std::string::npos);
}

TEST_CASE("Full pipeline: variable declaration", "[integration]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { int x = 10; return x; }");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $10, %rax") != std::string::npos);
    REQUIRE(result.assembly.find("-8(%rbp)") != std::string::npos);
}

TEST_CASE("Full pipeline: arithmetic", "[integration]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { return 2 + 3; }");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("add %rcx, %rax") != std::string::npos);
}

TEST_CASE("Full pipeline: function declaration", "[integration]") {
    Compiler compiler;
    auto result = compiler.compile("int add(int a, int b) { return a + b; }");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("add:") != std::string::npos);
}

TEST_CASE("Full pipeline: if statement", "[integration]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { if (1) return 1; }");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("cmp $0, %rax") != std::string::npos);
}

TEST_CASE("Full pipeline: while loop", "[integration]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { while (1) return 1; }");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("while_start") != std::string::npos);
}

TEST_CASE("Full pipeline: complex program", "[integration]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int add(int a, int b) {
            return a + b;
        }
        
        int main() {
            int x = 10;
            int y = 20;
            return add(x, y);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("add:") != std::string::npos);
    REQUIRE(result.assembly.find("main:") != std::string::npos);
}

TEST_CASE("Full pipeline: error handling", "[integration]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { return 42 }");
    REQUIRE_FALSE(result.success);
    REQUIRE_FALSE(result.error_message.empty());
}

TEST_CASE("Token output", "[integration]") {
    Lexer lexer("int x = 42;");
    auto tokens = lexer.tokenize();
    
    REQUIRE(tokens.size() == 6);
    REQUIRE(tokens[0].type == TokenType::KW_INT);
    REQUIRE(tokens[1].type == TokenType::IDENTIFIER);
    REQUIRE(tokens[2].type == TokenType::ASSIGN);
    REQUIRE(tokens[3].type == TokenType::INTEGER);
    REQUIRE(tokens[4].type == TokenType::SEMICOLON);
    REQUIRE(tokens[5].type == TokenType::EOF_TOKEN);
}

TEST_CASE("AST output", "[integration]") {
    Lexer lexer("int main() { return 0; }");
    auto tokens = lexer.tokenize();
    
    Parser parser(tokens);
    auto ast = parser.parse();
    auto program = std::unique_ptr<ProgramNode>(static_cast<ProgramNode*>(ast.release()));
    
    REQUIRE(program->type == NodeType::PROGRAM);
    REQUIRE(program->declarations.size() == 1);
    REQUIRE(program->declarations[0]->type == NodeType::FUNCTION_DECL);
}

TEST_CASE("Assembly output", "[integration]") {
    Lexer lexer("int main() { return 42; }");
    auto tokens = lexer.tokenize();
    
    Parser parser(tokens);
    auto ast = parser.parse();
    
    CodeGenerator codegen;
    auto assembly = codegen.generate(static_cast<ProgramNode&>(*ast));
    
    REQUIRE(assembly.find(".globl main") != std::string::npos);
    REQUIRE(assembly.find("main:") != std::string::npos);
    REQUIRE(assembly.find("mov $42, %rax") != std::string::npos);
    REQUIRE(assembly.find("ret") != std::string::npos);
}
