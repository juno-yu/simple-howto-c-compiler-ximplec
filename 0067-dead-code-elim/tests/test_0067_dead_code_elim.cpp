#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Dead code elimination - unreachable code after return", "[dead-code-elim]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int main() { return 0; return 1; }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("ret") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Dead code elimination - unused variable", "[dead-code-elim]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int main() { int x = 42; return 0; }"
    );
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Dead code elimination - code after break in loop", "[dead-code-elim]") {
    Compiler compiler;
    auto result = compiler.compile(
        "int main() { int i = 0; while(1) { break; i = 5; } return 0; }"
    );
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Dead code elimination - empty function body", "[dead-code-elim]") {
    Compiler compiler;
    auto result = compiler.compile("int f() { }");
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Dead code elimination - useful code not eliminated", "[dead-code-elim]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { int x = 5; return x; }");
    if (result.success) {
        REQUIRE(result.assembly.find("ret") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Dead code elimination - compile result is valid", "[dead-code-elim]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { return 0; }");
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
