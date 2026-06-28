#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Comma operator: basic sequential evaluation") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int a = (1, 2, 3);
            return a;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $3, %rax") != std::string::npos);
}

TEST_CASE("Comma operator: in for loop update") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int i = 0;
            int j = 10;
            for (i = 0, j = 10; i < j; i++, j--) {
            }
            return i;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("for_start") != std::string::npos);
}

TEST_CASE("Comma operator: side effects ordering") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 0;
            int y = (x = 5, x + 1);
            return y;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $5, %rax") != std::string::npos);
}

TEST_CASE("Comma operator: multiple expressions") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int a = (1 + 2, 3 * 4, 5 + 6);
            return a;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $11, %rax") != std::string::npos);
}

TEST_CASE("Comma operator: function call arguments") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int add(int a, int b) {
            return a + b;
        }
        int main() {
            int x = 5;
            return add(x, x);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("add:") != std::string::npos);
}

TEST_CASE("Comma operator: in return statement") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return (1, 2);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $2, %rax") != std::string::npos);
}

TEST_CASE("Comma operator: complex expressions") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 10;
            int y = 20;
            int z = (x = x + y, y = y - x, x + y);
            return z;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("add %rcx, %rax") != std::string::npos);
}

TEST_CASE("Comma operator: value is last expression") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int result = (42, 99);
            return result;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $99, %rax") != std::string::npos);
}
