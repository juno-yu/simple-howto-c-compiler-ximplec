#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Simple switch with cases", "[switch_case]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 1;
            switch (x) {
                case 1: return 10;
                case 2: return 20;
                case 3: return 30;
            }
            return 0;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $10, %rax") != std::string::npos);
}

TEST_CASE("Switch with default", "[switch_case]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 5;
            switch (x) {
                case 1: return 10;
                case 2: return 20;
                default: return 99;
            }
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $99, %rax") != std::string::npos);
}

TEST_CASE("Switch falls through to default", "[switch_case]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 3;
            switch (x) {
                case 1: return 10;
                case 2: return 20;
                default: return 77;
            }
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $77, %rax") != std::string::npos);
}

TEST_CASE("Switch with break", "[switch_case]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 2;
            int r = 0;
            switch (x) {
                case 1:
                    r = 10;
                    break;
                case 2:
                    r = 20;
                    break;
                case 3:
                    r = 30;
                    break;
                default:
                    r = 0;
                    break;
            }
            return r;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("switch") != std::string::npos);
}

TEST_CASE("Switch with multiple statements per case", "[switch_case]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 1;
            int a = 0;
            int b = 0;
            switch (x) {
                case 1:
                    a = 10;
                    b = 20;
                    break;
                case 2:
                    a = 30;
                    b = 40;
                    break;
                default:
                    a = 0;
                    b = 0;
                    break;
            }
            return a + b;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("add") != std::string::npos);
}

TEST_CASE("Switch with expression", "[switch_case]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 1 + 1;
            switch (x) {
                case 1: return 10;
                case 2: return 20;
                default: return 0;
            }
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $20, %rax") != std::string::npos);
}

TEST_CASE("Switch on zero", "[switch_case]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 0;
            switch (x) {
                case 0: return 42;
                case 1: return 1;
                default: return -1;
            }
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $42, %rax") != std::string::npos);
}

TEST_CASE("Switch with only default", "[switch_case]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 99;
            switch (x) {
                default: return 55;
            }
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $55, %rax") != std::string::npos);
}

TEST_CASE("Switch in loop", "[switch_case]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int sum = 0;
            int i = 0;
            while (i < 3) {
                switch (i) {
                    case 0: sum = sum + 1; break;
                    case 1: sum = sum + 2; break;
                    case 2: sum = sum + 3; break;
                    default: break;
                }
                i = i + 1;
            }
            return sum;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Switch with consecutive cases", "[switch_case]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 2;
            int r = 0;
            switch (x) {
                case 1:
                case 2:
                    r = 100;
                    break;
                case 3:
                    r = 200;
                    break;
                default:
                    r = 0;
                    break;
            }
            return r;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("switch") != std::string::npos);
}
