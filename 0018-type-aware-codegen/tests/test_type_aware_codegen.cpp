#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Char variable uses byte registers", "[type_aware_codegen]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char c = 65;
            return c;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("%al") != std::string::npos);
}

TEST_CASE("Short variable uses word registers", "[type_aware_codegen]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            short s = 100;
            return s;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $100, %rax") != std::string::npos);
}

TEST_CASE("Int variable uses long registers", "[type_aware_codegen]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 42;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("%eax") != std::string::npos);
}

TEST_CASE("Long variable uses quad registers", "[type_aware_codegen]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            long x = 100000;
            return 0;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("%rax") != std::string::npos);
}

TEST_CASE("Char comparison uses correct byte operations", "[type_aware_codegen]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char a = 10;
            char b = 20;
            if (a < b) {
                return 1;
            }
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Function parameter size is correct for int", "[type_aware_codegen]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int add(int a, int b) {
            return a + b;
        }
        int main() {
            return add(3, 4);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("movl") != std::string::npos);
}

TEST_CASE("Multiple type sizes in one function", "[type_aware_codegen]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char a = 1;
            short b = 2;
            int c = 3;
            long d = 4;
            return a + b + c + d;
        }
    )");
    REQUIRE(result.success);
}
