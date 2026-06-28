#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Ternary operator: basic true branch") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 1;
            int y = (x > 0) ? 10 : 20;
            return y;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("ternary_else") != std::string::npos);
    REQUIRE(result.assembly.find("ternary_end") != std::string::npos);
}

TEST_CASE("Ternary operator: basic false branch") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 0;
            int y = (x > 0) ? 10 : 20;
            return y;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("je ") != std::string::npos);
}

TEST_CASE("Ternary operator: jump structure") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return (1) ? 42 : 0;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("jmp ") != std::string::npos);
}

TEST_CASE("Ternary operator: nested ternary") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 5;
            int y = (x > 10) ? 100 : (x > 3) ? 50 : 0;
            return y;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("ternary_else") != std::string::npos);
}

TEST_CASE("Ternary operator: in return statement") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int abs_val(int x) {
            return (x >= 0) ? x : -x;
        }
        int main() {
            return abs_val(-5);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("abs_val:") != std::string::npos);
    REQUIRE(result.assembly.find("neg %rax") != std::string::npos);
}

TEST_CASE("Ternary operator: with comparison operators") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int max(int a, int b) {
            return (a > b) ? a : b;
        }
        int main() {
            return max(10, 20);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("max:") != std::string::npos);
    REQUIRE(result.assembly.find("setg %al") != std::string::npos);
}

TEST_CASE("Ternary operator: zero vs non-zero") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 0;
            return x ? 1 : 2;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("cmp $0, %rax") != std::string::npos);
}

TEST_CASE("Ternary operator: in assignment") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int a = 10;
            int b = 20;
            int result = (a < b) ? a : b;
            return result;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("setl %al") != std::string::npos);
}
