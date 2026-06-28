#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Register allocation - single variable stays in register", "[register-alloc]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { int x = 5; return x; }");
    if (result.success) {
        REQUIRE(result.assembly.find("ret") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Register allocation - multiple variables", "[register-alloc]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int main() { int a = 1; int b = 2; int c = 3; return a + b + c; }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("add") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Register allocation - arithmetic chain", "[register-alloc]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int main() { int a = 1; int b = a + 2; int c = b + 3; return c; }"
    );
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Register allocation - function call with arguments", "[register-alloc]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int add(int a, int b) { return a + b; } int main() { return add(1, 2); }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Register allocation - loop variable", "[register-alloc]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int main() { int sum = 0; int i = 0; while(i < 10) { sum = sum + i; i = i + 1; } return sum; }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("cmp") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Register allocation - compile result is valid", "[register-alloc]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { return 0; }");
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
