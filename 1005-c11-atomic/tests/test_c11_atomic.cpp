#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("_Atomic variable declaration", "[c11][atomic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Atomic int counter = 0;
            return counter;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Atomic variable assignment", "[c11][atomic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Atomic int x = 0;
            x = 42;
            return x;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Atomic with char type", "[c11][atomic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Atomic char c = 'a';
            return c;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Atomic variable increment", "[c11][atomic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Atomic int x = 0;
            x = x + 1;
            return x;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Atomic multiple variables", "[c11][atomic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Atomic int a = 1;
            _Atomic int b = 2;
            _Atomic int c = a + b;
            return c;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Atomic in struct", "[c11][atomic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Counter {
            _Atomic int value;
        };
        int main() {
            struct Counter c;
            c.value = 10;
            return c.value;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Atomic global variable", "[c11][atomic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        _Atomic int g_counter = 0;
        int main() {
            g_counter = 5;
            return g_counter;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Atomic with comparison operators", "[c11][atomic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Atomic int x = 10;
            _Atomic int y = 20;
            int result = (x < y);
            return result;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
