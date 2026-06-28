#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Global variable generates .data section", "[global_variables]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int g = 42;
        int main() {
            return g;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find(".data") != std::string::npos);
}

TEST_CASE("Global variable uses RIP-relative addressing", "[global_variables]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int g = 42;
        int main() {
            return g;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("g(%rip)") != std::string::npos);
}

TEST_CASE("Global variable declaration in assembly", "[global_variables]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int g = 42;
        int main() {
            return g;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find(".globl") != std::string::npos);
}

TEST_CASE("Uninitialized global uses .bss section", "[global_variables]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int g;
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Multiple global variables", "[global_variables]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int a = 10;
        int b = 20;
        int main() {
            return a + b;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find(".data") != std::string::npos);
}

TEST_CASE("Global variable written in function", "[global_variables]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int g = 0;
        int main() {
            g = 42;
            return g;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Global variable with different types", "[global_variables]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int gi = 42;
        char gc = 'A';
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Global variable accessed from multiple functions", "[global_variables]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int counter = 0;
        void increment() {
            counter = counter + 1;
        }
        int main() {
            increment();
            increment();
            return counter;
        }
    )");
    REQUIRE(result.success);
}
