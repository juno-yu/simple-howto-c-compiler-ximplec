#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Unsigned int declaration", "[signed_unsigned]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            unsigned int x = 42;
            return x;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Unsigned int with large value", "[signed_unsigned]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            unsigned int big = 4294967295;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Short type", "[signed_unsigned]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            short x = 100;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Long type", "[signed_unsigned]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            long x = 1000000;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Long long type", "[signed_unsigned]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            long long x = 9999999999;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Unsigned char", "[signed_unsigned]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            unsigned char c = 255;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Signed int explicitly", "[signed_unsigned]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            signed int x = -42;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Unsigned short", "[signed_unsigned]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            unsigned short x = 65535;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Unsigned long", "[signed_unsigned]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            unsigned long x = 1000000;
            return 0;
        }
    )");
    REQUIRE(result.success);
}
