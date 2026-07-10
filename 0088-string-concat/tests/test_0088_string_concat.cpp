#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
using namespace simplecc;

TEST_CASE("0088: string literal concatenation") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char *s = "hel" "lo";
            return s[1];
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("0088: multiple string concatenation") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char *s = "a" "b" "c" "d";
            return s[2];
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("0088: string concat with escape") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char *s = "hello" "\n";
            return s[0];
        }
    )");
    REQUIRE(result.success);
}
