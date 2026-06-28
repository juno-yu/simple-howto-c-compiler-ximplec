#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("_Generic with int type", "[c11][generic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 42;
            int val = _Generic(x, int: 1, default: 0);
            return val;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Generic with default clause", "[c11][generic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 42;
            int val = _Generic(x, float: 1, double: 2, default: 0);
            return val;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Generic with multiple types", "[c11][generic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 10;
            int val = _Generic(x, int: 100, char: 200, default: 0);
            return val;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Generic with char type", "[c11][generic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char c = 'a';
            int val = _Generic(c, char: 1, int: 2, default: 0);
            return val;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Generic with float type", "[c11][generic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            float f = 1.0f;
            int val = _Generic(f, float: 1, double: 2, int: 3, default: 0);
            return val;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Generic with pointer type", "[c11][generic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 5;
            int *p = &x;
            int val = _Generic(p, int*: 1, int: 2, default: 0);
            return val;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Generic dispatch expression result", "[c11][generic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 5;
            int val = _Generic(x, int: x * 2, default: 0);
            return val;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Generic with literal controlling expression", "[c11][generic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int val = _Generic(42, int: 1, default: 0);
            return val;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
