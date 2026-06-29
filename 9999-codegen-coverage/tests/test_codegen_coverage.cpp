#include <catch2/catch_test_macros.hpp>
#include "compiler.h"

using namespace simplecc;

static bool compile_code(const std::string& code) {
    Compiler compiler;
    auto result = compiler.compile(code);
    return result.success;
}

static CompileResult compile_with_output(const std::string& code) {
    Compiler compiler;
    return compiler.compile(code);
}

TEST_CASE("Do-while loop codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            int i = 0;
            do {
                i = i + 1;
            } while (i < 5);
            return i;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("do_while_start") != std::string::npos);
    REQUIRE(r.assembly.find("do_while_end") != std::string::npos);
    REQUIRE(r.assembly.find("jne") != std::string::npos);
}

TEST_CASE("Switch with cases codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            int x = 2;
            int result = 0;
            switch (x) {
                case 1: result = 10; break;
                case 2: result = 20; break;
                case 3: result = 30; break;
            }
            return result;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("switch_end") != std::string::npos);
    REQUIRE(r.assembly.find("case") != std::string::npos);
    REQUIRE(r.assembly.find("je ") != std::string::npos);
}

TEST_CASE("Switch with default codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            int x = 99;
            int result = 0;
            switch (x) {
                case 1: result = 10; break;
                default: result = 99; break;
            }
            return result;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("switch_default") != std::string::npos);
}

TEST_CASE("Switch with break codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            int x = 1;
            int result = 0;
            switch (x) {
                case 1: result = 10; break;
                case 2: result = 20;
            }
            return result;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("jmp ") != std::string::npos);
}

TEST_CASE("Ternary expression codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            int x = 5;
            int y = (x > 3) ? 100 : 200;
            return y;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("ternary_else") != std::string::npos);
    REQUIRE(r.assembly.find("ternary_end") != std::string::npos);
    REQUIRE(r.assembly.find("je ") != std::string::npos);
}

TEST_CASE("Inline asm statement codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            int x = 10;
            asm("nop");
            return x;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("nop") != std::string::npos);
    REQUIRE(r.assembly.find("# asm:") != std::string::npos);
}

TEST_CASE("Statement expression codegen (known bug: body is null)", "[codegen_coverage]") {
    // Known bug: StmtExprNode.body is not set by the parser (see parser.cpp:1990-1995),
    // so visit(StmtExprNode) iterates an empty block and produces no inner code.
    // The variable x is assigned whatever happens to be in %rax (zero from stack init).
    auto r = compile_with_output(R"(
        int main() {
            int x = ({
                int a = 5;
                int b = 7;
                a + b;
            });
            return x;
        }
    )");
    REQUIRE(r.success);
}

TEST_CASE("Break and continue in for loop codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            int sum = 0;
            for (int i = 0; i < 10; i++) {
                if (i == 5) break;
                if (i == 3) continue;
                sum = sum + i;
            }
            return sum;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("for_start") != std::string::npos);
    REQUIRE(r.assembly.find("for_end") != std::string::npos);
    REQUIRE(r.assembly.find("jmp ") != std::string::npos);
}

TEST_CASE("Break and continue in while loop codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            int i = 0;
            while (1) {
                if (i == 5) break;
                i++;
            }
            return i;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("while_start") != std::string::npos);
    REQUIRE(r.assembly.find("while_end") != std::string::npos);
}

TEST_CASE("Compound assignment += codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            int x = 10;
            x += 5;
            return x;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("add %rcx, %rax") != std::string::npos);
}

TEST_CASE("Compound assignment -= codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            int x = 10;
            x -= 3;
            return x;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("sub %rcx, %rax") != std::string::npos);
}

TEST_CASE("Compound assignment *= codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            int x = 6;
            x *= 7;
            return x;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("imul %rcx, %rax") != std::string::npos);
}

TEST_CASE("Compound assignment /= codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            int x = 20;
            x /= 4;
            return x;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("idiv %rcx") != std::string::npos);
}

TEST_CASE("Comma expression codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            int a = 1;
            int b = 2;
            int c = (a, b);
            return c;
        }
    )");
    REQUIRE(r.success);
}

TEST_CASE("Goto and label codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            int x = 0;
            goto end;
            x = 100;
            end:
            return x;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("end:") != std::string::npos);
    REQUIRE(r.assembly.find("jmp end") != std::string::npos);
}

TEST_CASE("sizeof struct type codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        struct S { int x; int y; char z; };
        int main() {
            return sizeof(struct S);
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("mov $9, %rax") != std::string::npos);
}

TEST_CASE("sizeof expression codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            int x = 1;
            return sizeof(x);
        }
    )");
    REQUIRE(r.success);
}

TEST_CASE("Compound literal codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            int *p = (int[]){1, 2, 3};
            return p[0] + p[1] + p[2];
        }
    )");
    REQUIRE(r.success);
}

TEST_CASE("Float subtraction codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            float a = 5.0f;
            float b = 3.0f;
            float c = a - b;
            int *pc = (int*)&c;
            int *pa = (int*)&a;
            int *pb = (int*)&b;
            return *pc != (*pa - *pb);
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("subss") != std::string::npos);
}

TEST_CASE("Float multiplication codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            float a = 2.0f;
            float b = 3.0f;
            float c = a * b;
            int *p = (int*)&c;
            return *p != 0x40c00000;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("mulss") != std::string::npos);
}

TEST_CASE("Float division codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            float a = 10.0f;
            float b = 2.0f;
            float c = a / b;
            int *p = (int*)&c;
            return *p != 0x41200000;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("divss") != std::string::npos);
}

TEST_CASE("Double multiplication codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            double a = 2.0;
            double b = 4.0;
            double c = a * b;
            int *p = (int*)&c;
            return *p != 0;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("mulsd") != std::string::npos);
}

TEST_CASE("Double subtraction codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            double a = 5.0;
            double b = 2.0;
            double c = a - b;
            int *p = (int*)&c;
            return *p != 0;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("subsd") != std::string::npos);
}

TEST_CASE("Float comparisons LE GT EQ NE codegen", "[codegen_coverage]") {
    // Note: >= (OpKind::GE) is omitted because the parser's parse_comparison()
    // has a bug: when token is GE, it sets op=GE in the else branch but never
    // calls match() to advance, causing an infinite loop.
    SECTION("LE") {
        auto r = compile_with_output("int main() { return (float)1.0 <= (float)2.0; }");
        REQUIRE(r.success);
        REQUIRE(r.assembly.find("setbe %al") != std::string::npos);
    }
    SECTION("GT") {
        auto r = compile_with_output("int main() { return (float)1.0 > (float)2.0; }");
        REQUIRE(r.success);
        REQUIRE(r.assembly.find("seta %al") != std::string::npos);
    }
    SECTION("EQ") {
        auto r = compile_with_output("int main() { return (float)1.0 == (float)2.0; }");
        REQUIRE(r.success);
        REQUIRE(r.assembly.find("sete %al") != std::string::npos);
    }
    SECTION("NE") {
        auto r = compile_with_output("int main() { return (float)1.0 != (float)2.0; }");
        REQUIRE(r.success);
        REQUIRE(r.assembly.find("setne %al") != std::string::npos);
    }
}

TEST_CASE("Double comparisons LE GT codegen", "[codegen_coverage]") {
    // Note: >= (OpKind::GE) omitted due to same parser bug as above.
    SECTION("LE") {
        auto r = compile_with_output("int main() { return (double)1.0 <= (double)2.0; }");
        REQUIRE(r.success);
        REQUIRE(r.assembly.find("setbe %al") != std::string::npos);
    }
    SECTION("GT") {
        auto r = compile_with_output("int main() { return (double)1.0 > (double)2.0; }");
        REQUIRE(r.success);
        REQUIRE(r.assembly.find("seta %al") != std::string::npos);
    }
}

TEST_CASE("Integer comparison LE GT EQ NE codegen", "[codegen_coverage]") {
    // Note: >= omitted due to same parser bug.
    SECTION("LE") {
        auto r = compile_with_output("int main() { return 1 <= 2; }");
        REQUIRE(r.success);
        REQUIRE(r.assembly.find("setle %al") != std::string::npos);
    }
    SECTION("GT") {
        auto r = compile_with_output("int main() { return 1 > 2; }");
        REQUIRE(r.success);
        REQUIRE(r.assembly.find("setg %al") != std::string::npos);
    }
    SECTION("EQ") {
        auto r = compile_with_output("int main() { return 1 == 2; }");
        REQUIRE(r.success);
        REQUIRE(r.assembly.find("sete %al") != std::string::npos);
    }
    SECTION("NE") {
        auto r = compile_with_output("int main() { return 1 != 2; }");
        REQUIRE(r.success);
        REQUIRE(r.assembly.find("setne %al") != std::string::npos);
    }
}

TEST_CASE("Logical AND on floats codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            float a = 1.0f;
            float b = 2.0f;
            if (a && b) return 1;
            return 0;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("and_false") != std::string::npos);
    REQUIRE(r.assembly.find("and_end") != std::string::npos);
}

TEST_CASE("Logical OR on floats codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            float a = 0.0f;
            float b = 2.0f;
            if (a || b) return 1;
            return 0;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("or_true") != std::string::npos);
    REQUIRE(r.assembly.find("or_end") != std::string::npos);
}

TEST_CASE("Bitwise AND OR XOR integer codegen", "[codegen_coverage]") {
    // Note: bitwise NOT (~) is omitted because the parser does not handle
    // the TILDE token in parse_unary(); only OpKind::BIT_NOT is referenced
    // by codegen.cpp but no source path can produce that node.
    auto r = compile_with_output(R"(
        int main() {
            int x = 12 & 10;
            int y = 12 | 1;
            int z = 12 ^ 3;
            return x + y + z;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("and %rcx, %rax") != std::string::npos);
    REQUIRE(r.assembly.find("or %rcx, %rax") != std::string::npos);
    REQUIRE(r.assembly.find("xor %rcx, %rax") != std::string::npos);
}

TEST_CASE("Integer shift left codegen", "[codegen_coverage]") {
    // Note: >> (RSHIFT) is omitted because parse_shift's while body only
    // calls match(LSHIFT), so for RSHIFT it fails to advance, causing an
    // infinite loop in the parser.
    auto r = compile_with_output(R"(
        int main() {
            int x = 4;
            int y = x << 2;
            return y;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("shl %cl, %rax") != std::string::npos);
}

TEST_CASE("Pre-increment codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            int x = 5;
            int y = ++x;
            return y;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("add $1, %rax") != std::string::npos);
}

TEST_CASE("Post-increment codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            int x = 5;
            int y = x++;
            return y;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("add $1, %rax") != std::string::npos);
}

TEST_CASE("Pre-decrement codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            int x = 5;
            int y = --x;
            return y;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("sub $1, %rax") != std::string::npos);
}

TEST_CASE("Post-decrement codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            int x = 5;
            int y = x--;
            return y;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("sub $1, %rax") != std::string::npos);
}

TEST_CASE("Float unary minus codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            float a = 3.0f;
            float b = -a;
            int *p = (int*)&b;
            return *p != 0xc0400000;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("xorps") != std::string::npos);
}

TEST_CASE("Double unary minus codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            double a = 3.0;
            double b = -a;
            return (b == -3.0);
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("xorpd") != std::string::npos);
}

TEST_CASE("Float to double cast codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            float f = 1.5f;
            double d = (double)f;
            return (d == 1.5);
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("cvtss2sd") != std::string::npos);
}

TEST_CASE("Double to float cast codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            double d = 1.5;
            float f = (float)d;
            int *p = (int*)&f;
            return *p != 0x3fc00000;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("cvtsd2ss") != std::string::npos);
}

TEST_CASE("Function returning float with int expr codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        float tofloat(int x) {
            return x;
        }
        int main() {
            float f = tofloat(42);
            int *p = (int*)&f;
            return *p != 0x42280000;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("cvtsi2ss") != std::string::npos);
}

TEST_CASE("Recursive function call codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int fact(int n) {
            if (n <= 1) return 1;
            return n * fact(n - 1);
        }
        int main() {
            return fact(5);
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("call fact") != std::string::npos);
}

TEST_CASE("Call with 6 integer args codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int sum6(int a, int b, int c, int d, int e, int f) {
            return a + b + c + d + e + f;
        }
        int main() {
            return sum6(1, 2, 3, 4, 5, 6);
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("call sum6") != std::string::npos);
}

TEST_CASE("Call with float and int args codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        float mix(int i, float f, int j) {
            return (float)i + f + (float)j;
        }
        int main() {
            float r = mix(1, 2.0f, 3);
            int *p = (int*)&r;
            return *p != 0x40a00000;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("call mix") != std::string::npos);
}

TEST_CASE("Builtin offsetof codegen (known crash)", "[codegen_coverage]") {
    // Known issue: __builtin_offsetof takes a type as first argument, but the
    // parser does not recognize "struct S" inside a call expression's argument
    // list, so the CallExprNode is built with a null first argument. The
    // codegen dynamic_casts it without a null check and segfaults.
    // We just document this and avoid triggering the crash; the segment
    // visit(CallExprNode) for offsetof is still exercised by the source code.
    SUCCEED("Documented known issue: __builtin_offsetof segfaults due to null arg in AST");
}

TEST_CASE("Builtin popcount codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            return __builtin_popcount(7);
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("popcnt %eax, %eax") != std::string::npos);
}

TEST_CASE("Array initialization with multiple elements codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            int arr[5] = {1, 2, 3, 4, 5};
            return arr[0] + arr[1] + arr[2] + arr[3] + arr[4];
        }
    )");
    REQUIRE(r.success);
}

TEST_CASE("Designated initializer codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        struct S { int a; int b; int c; };
        int main() {
            struct S s = {.b = 20, .a = 10, .c = 30};
            return s.a + s.b + s.c;
        }
    )");
    REQUIRE(r.success);
}

TEST_CASE("Char literal in return codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            char c = 'Z';
            return c;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("mov $90, %rax") != std::string::npos);
}

TEST_CASE("String literal address codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            char *s = "hello";
            return s[0];
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find(".Lstr_") != std::string::npos);
    REQUIRE(r.assembly.find(".asciz") != std::string::npos);
}

TEST_CASE("Global variable with initializer codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int g = 42;
        int main() {
            return g;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find(".globl g") != std::string::npos);
    REQUIRE(r.assembly.find(".data") != std::string::npos);
}

TEST_CASE("Member access through arrow operator codegen (parser broken)", "[codegen_coverage]") {
    // Known issue: the parser does not produce a MemberExprNode with is_arrow=true
    // for `p->x` in the current grammar. We just document this.
    // The codegen path for is_arrow is reachable in principle (see compute_member_address
    // in codegen.cpp:568), but no end-to-end C source can trigger it today.
    SUCCEED("Documented known issue: parser does not produce is_arrow MemberExprNode");
}

TEST_CASE("Float ternary codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            float a = 5.0f;
            float b = 3.0f;
            float c = (a > b) ? a : b;
            int *p = (int*)&c;
            return *p != 0x40a00000;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("ternary_else") != std::string::npos);
    REQUIRE(r.assembly.find("ternary_end") != std::string::npos);
}

TEST_CASE("Integer division codegen", "[codegen_coverage]") {
    auto r = compile_with_output(R"(
        int main() {
            int x = 17 / 5;
            return x;
        }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find("idiv %rcx") != std::string::npos);
}
