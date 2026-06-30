#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
using namespace simplecc;

TEST_CASE("GE operator no longer hangs (parse_comparison)", "[bugfix]") {
    Compiler c;
    auto r = c.compile("int main() { return 1 >= 1; }");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("setge") != std::string::npos);
}

TEST_CASE("LE operator no longer hangs (parse_comparison)", "[bugfix]") {
    Compiler c;
    auto r = c.compile("int main() { return 1 <= 1; }");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("setle") != std::string::npos);
}

TEST_CASE("GT operator no longer hangs (parse_comparison)", "[bugfix]") {
    Compiler c;
    auto r = c.compile("int main() { return 1 > 0; }");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("setg") != std::string::npos);
}

TEST_CASE("LT operator still works (parse_comparison)", "[bugfix]") {
    Compiler c;
    auto r = c.compile("int main() { return 1 < 2; }");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("setl") != std::string::npos);
}

TEST_CASE("RSHIFT operator no longer hangs (parse_shift)", "[bugfix]") {
    Compiler c;
    auto r = c.compile("int main() { return 16 >> 2; }");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("shr") != std::string::npos);
}

TEST_CASE("LSHIFT operator still works (parse_shift)", "[bugfix]") {
    Compiler c;
    auto r = c.compile("int main() { return 1 << 4; }");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("shl") != std::string::npos);
}

TEST_CASE("MOD operator no longer hangs (parse_multiplication)", "[bugfix]") {
    Compiler c;
    auto r = c.compile("int main() { return 10 % 3; }");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("idiv") != std::string::npos);
}

TEST_CASE("Modulo compound assign %= works", "[bugfix]") {
    Compiler c;
    auto r = c.compile("int main() { int x = 10; x %= 3; return x; }");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("idiv") != std::string::npos);
    REQUIRE(r.assembly.find("mov %rdx, %rax") != std::string::npos);
}

TEST_CASE("Bitwise AND compound assign &= works", "[bugfix]") {
    Compiler c;
    auto r = c.compile("int main() { int x = 0xFF; x &= 0x0F; return x; }");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("and") != std::string::npos);
}

TEST_CASE("Bitwise OR compound assign |= works", "[bugfix]") {
    Compiler c;
    auto r = c.compile("int main() { int x = 0x0F; x |= 0xF0; return x; }");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("or") != std::string::npos);
}

TEST_CASE("Bitwise XOR compound assign ^= works", "[bugfix]") {
    Compiler c;
    auto r = c.compile("int main() { int x = 0xFF; x ^= 0x0F; return x; }");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("xor") != std::string::npos);
}

TEST_CASE("Left shift compound assign <<= works", "[bugfix]") {
    Compiler c;
    auto r = c.compile("int main() { int x = 1; x <<= 4; return x; }");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("shl") != std::string::npos);
}

TEST_CASE("Right shift compound assign >>= works", "[bugfix]") {
    Compiler c;
    auto r = c.compile("int main() { int x = 16; x >>= 2; return x; }");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("shr") != std::string::npos);
}

TEST_CASE("Bitwise NOT ~ is now reachable in parser", "[bugfix]") {
    Compiler c;
    auto r = c.compile("int main() { int x = 0; x = ~x; return x; }");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("not %rax") != std::string::npos);
}

TEST_CASE("StmtExprNode body is now set in parser", "[bugfix]") {
    Compiler c;
    auto r = c.compile("int main() { int x = ({ int a = 5; int b = 10; a + b; }); return x; }");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("mov $5, %rax") != std::string::npos);
    REQUIRE(r.assembly.find("mov $10, %rax") != std::string::npos);
    REQUIRE(r.assembly.find("add") != std::string::npos);
}

TEST_CASE("Struct pointer declaration now works", "[bugfix]") {
    Compiler c;
    auto r = c.compile(R"(
        struct N { int v; };
        int main() {
            struct N n;
            n.v = 42;
            struct N *p = &n;
            return p->v;
        }
    )");
    REQUIRE(r.success);
}

TEST_CASE("Union pointer declaration now works", "[bugfix]") {
    Compiler c;
    auto r = c.compile(R"(
        union U { int i; char c; };
        int main() {
            union U u;
            u.i = 65;
            union U *p = &u;
            return p->i;
        }
    )");
    REQUIRE(r.success);
}

TEST_CASE("Arrow operator -> is now lexed correctly", "[bugfix]") {
    Compiler c;
    auto r = c.compile(R"(
        struct N { int v; };
        int main() {
            struct N n;
            n.v = 42;
            struct N *p = &n;
            return p->v;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("->") == std::string::npos);
    REQUIRE(r.assembly.find("mov") != std::string::npos);
}

TEST_CASE("Scientific notation 1.5e10 in float literal", "[bugfix]") {
    Compiler c;
    auto r = c.compile(R"(
        int main() {
            double d = 1.5e10;
            return 0;
        }
    )");
    REQUIRE(r.success);
}

TEST_CASE("Scientific notation 1.5E-3 in float literal", "[bugfix]") {
    Compiler c;
    auto r = c.compile(R"(
        int main() {
            double d = 1.5E-3;
            return 0;
        }
    )");
    REQUIRE(r.success);
}

TEST_CASE("Scientific notation with integer base 1e10", "[bugfix]") {
    Compiler c;
    auto r = c.compile(R"(
        int main() {
            double d = 1e10;
            return 0;
        }
    )");
    REQUIRE(r.success);
}

TEST_CASE("parse_expression no longer segfaults on error + comma", "[bugfix]") {
    Compiler c;
    auto r = c.compile("int main() { int x = struct, 5; return 0; }");
    REQUIRE_FALSE(r.success);
}

TEST_CASE("Float compound assign += uses SSE addss", "[bugfix]") {
    Compiler c;
    auto r = c.compile(R"(
        int main() {
            float a = 1.0f;
            a += 2.0f;
            return 0;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("addss") != std::string::npos);
}

TEST_CASE("Float compound assign -= uses SSE subss", "[bugfix]") {
    Compiler c;
    auto r = c.compile(R"(
        int main() {
            float a = 5.0f;
            a -= 2.0f;
            return 0;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("subss") != std::string::npos);
}

TEST_CASE("Float compound assign *= uses SSE mulss", "[bugfix]") {
    Compiler c;
    auto r = c.compile(R"(
        int main() {
            float a = 3.0f;
            a *= 2.0f;
            return 0;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("mulss") != std::string::npos);
}

TEST_CASE("Float compound assign /= uses SSE divss", "[bugfix]") {
    Compiler c;
    auto r = c.compile(R"(
        int main() {
            float a = 6.0f;
            a /= 2.0f;
            return 0;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("divss") != std::string::npos);
}

TEST_CASE("Float compound assign with int RHS promotes", "[bugfix]") {
    Compiler c;
    auto r = c.compile(R"(
        int main() {
            float a = 1.0f;
            a += 2;
            return 0;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("cvtsi2ss") != std::string::npos);
    REQUIRE(r.assembly.find("addss") != std::string::npos);
}

TEST_CASE("Double compound assign += uses SSE addsd", "[bugfix]") {
    Compiler c;
    auto r = c.compile(R"(
        int main() {
            double a = 1.0;
            a += 2.0;
            return 0;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("addsd") != std::string::npos);
}
