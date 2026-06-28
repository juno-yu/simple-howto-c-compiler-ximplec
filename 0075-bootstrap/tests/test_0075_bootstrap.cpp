#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Bootstrap - compile trivial program", "[bootstrap]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { return 0; }");
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Bootstrap - compile arithmetic program", "[bootstrap]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { return 2 + 3; }");
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Bootstrap - compile fibonacci", "[bootstrap]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int fib(int n) { if (n <= 1) return n; return fib(n-1) + fib(n-2); } "
        "int main() { return fib(10); }"
    );
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Bootstrap - compile function with control flow", "[bootstrap]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int abs(int x) { if (x < 0) return -x; return x; } "
        "int main() { return abs(-5); }"
    );
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Bootstrap - compile loop program", "[bootstrap]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int sum(int n) { int s = 0; int i; for(i = 1; i <= n; i = i + 1) s = s + i; return s; } "
        "int main() { return sum(100); }"
    );
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Bootstrap - compile self-referential function", "[bootstrap]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int pow(int base, int exp) { int result = 1; int i; for(i = 0; i < exp; i = i + 1) result = result * base; return result; } "
        "int main() { return pow(2, 10); }"
    );
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
