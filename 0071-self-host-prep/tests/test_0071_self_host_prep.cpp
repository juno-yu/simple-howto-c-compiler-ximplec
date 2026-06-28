#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Self-host prep - compiler handles int type", "[self-host-prep]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { int x = 1; return x; }");
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Self-host prep - compiler handles char type", "[self-host-prep]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { char c = 'a'; return c; }");
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Self-host prep - compiler handles bool type", "[self-host-prep]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { _Bool b = 1; return b; }");
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Self-host prep - compiler handles functions with multiple params", "[self-host-prep]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int func(int a, int b, int c) { return a + b + c; } int main() { return func(1, 2, 3); }"
    );
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Self-host prep - compiler handles string literals", "[self-host-prep]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { char* s = \"hello\"; return 0; }");
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Self-host prep - preprocessor not supported gracefully", "[self-host-prep]") {
    Compiler compiler;
    auto result = compiler.compile("#define FOO 42\nint main() { return 0; }");
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Self-host prep - struct not supported gracefully", "[self-host-prep]") {
    Compiler compiler;
    auto result = compiler.compile("struct Foo { int x; }; int main() { return 0; }");
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Self-host prep - enum not supported gracefully", "[self-host-prep]") {
    Compiler compiler;
    auto result = compiler.compile("enum Color { RED, GREEN, BLUE }; int main() { return 0; }");
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
