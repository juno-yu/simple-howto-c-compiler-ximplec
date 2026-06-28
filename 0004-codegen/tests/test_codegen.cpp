#include <catch2/catch_test_macros.hpp>
#include "codegen.h"
#include "parser.h"
#include "lexer.h"

using namespace simplecc;

std::string generate_code(const std::string& source) {
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    REQUIRE_FALSE(lexer.has_error());
    
    Parser parser(tokens);
    auto ast = parser.parse();
    REQUIRE_FALSE(parser.has_error());
    
    CodeGenerator codegen;
    auto assembly = codegen.generate(static_cast<ProgramNode&>(*ast));
    REQUIRE_FALSE(codegen.has_error());
    
    return assembly;
}

TEST_CASE("Generate simple return", "[codegen]") {
    auto asm_code = generate_code("int main() { return 42; }");
    REQUIRE(asm_code.find(".globl main") != std::string::npos);
    REQUIRE(asm_code.find("main:") != std::string::npos);
    REQUIRE(asm_code.find("mov $42, %rax") != std::string::npos);
    REQUIRE(asm_code.find("ret") != std::string::npos);
}

TEST_CASE("Generate variable declaration", "[codegen]") {
    auto asm_code = generate_code("int main() { int x = 42; return x; }");
    REQUIRE(asm_code.find("mov $42, %rax") != std::string::npos);
    REQUIRE(asm_code.find("mov %rax, -8(%rbp)") != std::string::npos);
    REQUIRE(asm_code.find("mov -8(%rbp), %rax") != std::string::npos);
}

TEST_CASE("Generate addition", "[codegen]") {
    auto asm_code = generate_code("int main() { return 2 + 3; }");
    REQUIRE(asm_code.find("mov $2, %rax") != std::string::npos);
    REQUIRE(asm_code.find("push %rax") != std::string::npos);
    REQUIRE(asm_code.find("mov $3, %rax") != std::string::npos);
    REQUIRE(asm_code.find("pop %rcx") != std::string::npos);
    REQUIRE(asm_code.find("add %rcx, %rax") != std::string::npos);
}

TEST_CASE("Generate multiplication", "[codegen]") {
    auto asm_code = generate_code("int main() { return 2 * 3; }");
    REQUIRE(asm_code.find("imul %rcx, %rax") != std::string::npos);
}

TEST_CASE("Generate comparison", "[codegen]") {
    auto asm_code = generate_code("int main() { return 2 < 3; }");
    REQUIRE(asm_code.find("cmp %rcx, %rax") != std::string::npos);
    REQUIRE(asm_code.find("setl %al") != std::string::npos);
}

TEST_CASE("Generate if statement", "[codegen]") {
    auto asm_code = generate_code("int main() { if (1) return 1; }");
    REQUIRE(asm_code.find("cmp $0, %rax") != std::string::npos);
    REQUIRE(asm_code.find("je ") != std::string::npos);
}

TEST_CASE("Generate while loop", "[codegen]") {
    auto asm_code = generate_code("int main() { while (1) return 1; }");
    REQUIRE(asm_code.find("while_start") != std::string::npos);
    REQUIRE(asm_code.find("while_end") != std::string::npos);
    REQUIRE(asm_code.find("jmp ") != std::string::npos);
}

TEST_CASE("Generate function call", "[codegen]") {
    auto asm_code = generate_code("int main() { return foo(); }");
    REQUIRE(asm_code.find("call foo") != std::string::npos);
}

TEST_CASE("Generate function prologue/epilogue", "[codegen]") {
    auto asm_code = generate_code("int main() { return 0; }");
    REQUIRE(asm_code.find("push %rbp") != std::string::npos);
    REQUIRE(asm_code.find("mov %rsp, %rbp") != std::string::npos);
    REQUIRE(asm_code.find("mov %rbp, %rsp") != std::string::npos);
    REQUIRE(asm_code.find("pop %rbp") != std::string::npos);
}

TEST_CASE("Generate multiple functions", "[codegen]") {
    auto asm_code = generate_code(
        "int add(int a, int b) { return a + b; }"
        "int main() { return add(1, 2); }"
    );
    REQUIRE(asm_code.find("add:") != std::string::npos);
    REQUIRE(asm_code.find("main:") != std::string::npos);
}

TEST_CASE("Generate string literal", "[codegen]") {
    auto asm_code = generate_code("int main() { char s = 'a'; }");
    REQUIRE_FALSE(asm_code.empty());
}
