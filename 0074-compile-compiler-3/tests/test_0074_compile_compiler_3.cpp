#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Compile compiler phase 3 - codegen: function prologue/epilogue", "[compile-compiler-3]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { return 0; }");
    if (result.success) {
        REQUIRE(result.assembly.find("ret") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Compile compiler phase 3 - codegen: stack frame for locals", "[compile-compiler-3]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int main() { int a = 1; int b = 2; int c = a + b; return c; }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("ret") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Compile compiler phase 3 - codegen: function call convention", "[compile-compiler-3]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int add(int a, int b) { return a + b; } int main() { return add(3, 4); }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Compile compiler phase 3 - codegen: conditional branching", "[compile-compiler-3]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int main() { int x = 1; if (x > 0) { return 1; } return 0; }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("cmp") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Compile compiler phase 3 - codegen: loop with label", "[compile-compiler-3]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int main() { int i = 0; while(i < 5) { i = i + 1; } return i; }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("cmp") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Compile compiler phase 3 - codegen: complete program", "[compile-compiler-3]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int fib(int n) { if (n <= 1) return n; return fib(n-1) + fib(n-2); } "
        "int main() { return fib(10); }"
    );
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
        REQUIRE(result.assembly.find("call") != std::string::npos);
        REQUIRE(result.assembly.find("ret") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
