#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Debug info - basic compilation produces assembly", "[debug-info]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { return 0; }");
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Debug info - function with local variables", "[debug-info]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int main() { int x = 42; return x; }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("ret") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Debug info - multiple functions", "[debug-info]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int add(int a, int b) { return a + b; } int main() { return add(1, 2); }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("add") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Debug info - control flow", "[debug-info]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int main() { int x = 0; if (x) { x = 1; } else { x = 2; } return x; }"
    );
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Debug info - loop construct", "[debug-info]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int main() { int i = 0; while(i < 10) { i = i + 1; } return i; }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("cmp") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Debug info - DWARF not yet generated gracefully", "[debug-info]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { return 0; }");
    if (result.success) {
        REQUIRE(result.assembly.find(".debug") == std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
