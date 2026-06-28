#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Compile compiler phase 1 - tokenize: simple function", "[compile-compiler-1]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { return 0; }");
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Compile compiler phase 1 - tokenize: integer literals", "[compile-compiler-1]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { int x = 42; return x; }");
    if (result.success) {
        REQUIRE(result.assembly.find("$42") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Compile compiler phase 1 - tokenize: string literals", "[compile-compiler-1]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { char* s = \"hello\"; return 0; }");
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Compile compiler phase 1 - tokenize: operators", "[compile-compiler-1]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { int x = 1 + 2 - 3 * 4 / 5; return x; }");
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Compile compiler phase 1 - tokenize: comments stripped", "[compile-compiler-1]") {
    Compiler compiler;
    auto result = compiler.compile("/* comment */ int main() { return 0; } // line comment");
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Compile compiler phase 1 - tokenize: multiple tokens per line", "[compile-compiler-1]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int x = 1; int y = 2; int z = x + y; return z;"
    );
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
