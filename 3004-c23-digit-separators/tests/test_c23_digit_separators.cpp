#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("C23 digit separators: single quote in integer") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 1'000;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $1000, %rax") != std::string::npos);
}

TEST_CASE("C23 digit separators: million number") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 1'000'000;
            return x;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 digit separators: hex with separators") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 0xFF'FF;
            return x;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 digit separators: binary with separators") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 0b1010'0101;
            return x;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 digit separators: octal with separators") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 0'777;
            return x;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 digit separators: multiple consecutive separators") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 1''2''3;
            return x;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 digit separators: separator at start invalid") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = '123;
            return x;
        }
    )");
    REQUIRE_FALSE(result.success);
}

TEST_CASE("C23 digit separators: separator at end invalid") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 123';
            return x;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 digit separators: large constant") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 1'234'567'890;
            return x;
        }
    )");
    REQUIRE(result.success);
}
