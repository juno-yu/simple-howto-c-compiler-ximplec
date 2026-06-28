#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Define macro without preprocessor support", "[preprocessor_macros]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #define PI 314
        int main() {
            return 42;
        }
    )");
    REQUIRE_FALSE(result.success);
    REQUIRE_FALSE(result.error_message.empty());
}

TEST_CASE("Function-like macro without preprocessor support", "[preprocessor_macros]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #define MAX(a, b) ((a) > (b) ? (a) : (b))
        int main() {
            return MAX(10, 20);
        }
    )");
    REQUIRE_FALSE(result.success);
    REQUIRE_FALSE(result.error_message.empty());
}

TEST_CASE("Undef macro without preprocessor support", "[preprocessor_macros]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #define VALUE 42
        #undef VALUE
        int main() {
            return 0;
        }
    )");
    REQUIRE_FALSE(result.success);
    REQUIRE_FALSE(result.error_message.empty());
}

TEST_CASE("Macro with no arguments", "[preprocessor_macros]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #define HELLO 1
        int main() {
            return HELLO;
        }
    )");
    REQUIRE_FALSE(result.success);
}

TEST_CASE("Nested macro definition", "[preprocessor_macros]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #define A 1
        #define B A
        int main() {
            return B;
        }
    )");
    REQUIRE_FALSE(result.success);
}

TEST_CASE("Empty source with only define", "[preprocessor_macros]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #define FOO
    )");
    REQUIRE_FALSE(result.success);
}
