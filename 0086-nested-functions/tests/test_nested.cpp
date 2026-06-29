#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "compiler.h"

using namespace simplecc;

static bool compile_code(const std::string& code) {
    Compiler compiler;
    auto result = compiler.compile(code);
    return result.success;
}

TEST_CASE("0086 nested function declaration", "[lessons][0086]") {
    REQUIRE(compile_code(R"(
        int main() {
            int add(int x) { return x + 1; }
            return add(5);
        }
    )"));
}

TEST_CASE("0086 nested function with capture", "[lessons][0086]") {
    REQUIRE(compile_code(R"(
        int main() {
            int y = 10;
            int add(int x) { return x + y; }
            return add(5);
        }
    )"));
}

TEST_CASE("0086 nested function with no capture simple", "[lessons][0086]") {
    REQUIRE(compile_code(R"(
        int main() {
            int f(int x) { return x*2; }
            return f(21);
        }
    )"));
}

TEST_CASE("0086 nested function with int param", "[lessons][0086]") {
    REQUIRE(compile_code(R"(
        int main() {
            int square(int x) { return x*x; }
            return square(7);
        }
    )"));
}
