#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("C23 binary literals: simple binary") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 0b1010;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $10, %rax") != std::string::npos);
}

TEST_CASE("C23 binary literals: all zeros") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 0b0000;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $0, %rax") != std::string::npos);
}

TEST_CASE("C23 binary literals: all ones") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 0b1111;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $15, %rax") != std::string::npos);
}

TEST_CASE("C23 binary literals: single bit") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 0b1;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $1, %rax") != std::string::npos);
}

TEST_CASE("C23 binary literals: uppercase B") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 0B1010;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $10, %rax") != std::string::npos);
}

TEST_CASE("C23 binary literals: with digit separators") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 0b1010'0101;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $165, %rax") != std::string::npos);
}

TEST_CASE("C23 binary literals: large binary") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 0b11111111;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $255, %rax") != std::string::npos);
}

TEST_CASE("C23 binary literals: in conditional") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            if (0b1010 > 0b0101) return 1;
            return 0;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $1, %rax") != std::string::npos);
}

TEST_CASE("C23 binary literals: in arithmetic") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return 0b1010 + 0b0101;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("add") != std::string::npos);
}
