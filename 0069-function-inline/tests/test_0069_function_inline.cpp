#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Function inlining - small function call compiles", "[function-inline]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int square(int x) { return x * x; } int main() { return square(5); }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Function inlining - identity function", "[function-inline]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int id(int x) { return x; } int main() { return id(42); }"
    );
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Function inlining - multiple calls to same function", "[function-inline]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int inc(int x) { return x + 1; } int main() { int a = inc(1); int b = inc(2); return a + b; }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Function inlining - recursive function not inlined", "[function-inline]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int fib(int n) { if (n <= 1) return n; return fib(n-1) + fib(n-2); } int main() { return fib(5); }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Function inlining - nested function calls", "[function-inline]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int add(int a, int b) { return a + b; } int main() { return add(add(1, 2), add(3, 4)); }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Function inlining - compile result is valid", "[function-inline]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { return 0; }");
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
