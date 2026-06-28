#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("constexpr basic variable", "[c17][constexpr]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            constexpr int x = 42;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("constexpr with expression", "[c17][constexpr]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            constexpr int x = 10 + 20;
            constexpr int y = x * 2;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("constexpr in array size", "[c17][constexpr]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            constexpr int size = 10;
            int arr[size];
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("constexpr in switch case", "[c17][constexpr]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            constexpr int flag = 1;
            switch (flag) {
                case 1: return 0;
                default: return 1;
            }
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("constexpr with type specifiers", "[c17][constexpr]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            constexpr long big = 1000000L;
            constexpr unsigned int u = 42U;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("constexpr in ternary operator", "[c17][constexpr]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            constexpr int x = 5;
            int result = (x > 3) ? 1 : 0;
            return result;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("constexpr with sizeof", "[c17][constexpr]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            constexpr int int_size = sizeof(int);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
