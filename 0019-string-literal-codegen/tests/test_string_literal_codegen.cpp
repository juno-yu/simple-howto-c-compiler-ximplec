#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("String literal generates .rodata section", "[string_literal_codegen]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char *s = "hello";
            return 0;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find(".rodata") != std::string::npos);
}

TEST_CASE("String literal uses RIP-relative lea", "[string_literal_codegen]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char *s = "hello";
            return 0;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("lea") != std::string::npos);
    REQUIRE(result.assembly.find("(%rip)") != std::string::npos);
}

TEST_CASE("String literal has label", "[string_literal_codegen]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char *s = "hello";
            return 0;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find(".Lstr") != std::string::npos);
}

TEST_CASE("String literal with escape sequences", "[string_literal_codegen]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char *s = "hello\nworld";
            return 0;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find(".rodata") != std::string::npos);
}

TEST_CASE("Multiple string literals", "[string_literal_codegen]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char *a = "first";
            char *b = "second";
            return 0;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find(".rodata") != std::string::npos);
}

TEST_CASE("Empty string literal", "[string_literal_codegen]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char *s = "";
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("String literal used in function", "[string_literal_codegen]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return "hello"[0];
        }
    )");
    REQUIRE(result.success);
}
