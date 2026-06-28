#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Const integer declaration", "[const_qualifier]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            const int x = 10;
            return x;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Const variable initialization", "[const_qualifier]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            const int MAX = 100;
            return MAX;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $100, %rax") != std::string::npos);
}

TEST_CASE("Const char declaration", "[const_qualifier]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            const char c = 'A';
            return c;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Const used in expression", "[const_qualifier]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            const int x = 5;
            const int y = 10;
            return x + y;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("add") != std::string::npos);
}

TEST_CASE("Const pointer declaration", "[const_qualifier]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int val = 42;
            int *const p = &val;
            return *p;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Pointer to const", "[const_qualifier]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            const int x = 5;
            const int *p = &x;
            return *p;
        }
    )");
    REQUIRE(result.success);
}
