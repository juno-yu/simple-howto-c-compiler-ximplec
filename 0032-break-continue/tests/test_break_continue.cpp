#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Break in while loop", "[break_continue]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int i = 0;
            while (i < 10) {
                if (i == 5) break;
                i = i + 1;
            }
            return i;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("jmp") != std::string::npos);
}

TEST_CASE("Continue in while loop", "[break_continue]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int i = 0;
            int sum = 0;
            while (i < 10) {
                i = i + 1;
                if (i % 2 == 0) continue;
                sum = sum + i;
            }
            return sum;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Break in for loop", "[break_continue]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int sum = 0;
            int i;
            for (i = 0; i < 100; i = i + 1) {
                if (i == 10) break;
                sum = sum + i;
            }
            return sum;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Continue in for loop", "[break_continue]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int sum = 0;
            int i;
            for (i = 0; i < 10; i = i + 1) {
                if (i % 2 == 0) continue;
                sum = sum + i;
            }
            return sum;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Break in do-while loop", "[break_continue]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int i = 0;
            do {
                if (i == 5) break;
                i = i + 1;
            } while (i < 10);
            return i;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Nested loop break", "[break_continue]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int i = 0;
            while (i < 10) {
                int j = 0;
                while (j < 10) {
                    if (j == 3) break;
                    j = j + 1;
                }
                i = i + 1;
            }
            return 42;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Nested loop continue", "[break_continue]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int sum = 0;
            int i;
            for (i = 0; i < 5; i = i + 1) {
                int j;
                for (j = 0; j < 5; j = j + 1) {
                    if (j % 2 == 0) continue;
                    sum = sum + 1;
                }
            }
            return sum;
        }
    )");
    REQUIRE(result.success);
}
