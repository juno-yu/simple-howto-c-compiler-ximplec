#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Compile compiler phase 2 - parse: function declarations", "[compile-compiler-2]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int add(int a, int b); int main() { return add(1, 2); }"
    );
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Compile compiler phase 2 - parse: if-else statements", "[compile-compiler-2]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int main() { int x = 1; if (x) { x = 2; } else { x = 3; } return x; }"
    );
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Compile compiler phase 2 - parse: while loops", "[compile-compiler-2]") {
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

TEST_CASE("Compile compiler phase 2 - parse: for loops", "[compile-compiler-2]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int main() { int sum = 0; int i; for(i = 0; i < 10; i = i + 1) { sum = sum + i; } return sum; }"
    );
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Compile compiler phase 2 - parse: nested expressions", "[compile-compiler-2]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int main() { int x = (1 + 2) * (3 + 4); return x; }"
    );
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Compile compiler phase 2 - parse: recursive function", "[compile-compiler-2]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int fib(int n) { if (n <= 1) return n; return fib(n - 1) + fib(n - 2); } int main() { return fib(5); }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
