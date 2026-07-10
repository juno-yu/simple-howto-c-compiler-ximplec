#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
using namespace simplecc;

TEST_CASE("0089: for loop with comma in init") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int i, s = 0;
            for (i = 0, s = 0; i < 3; i++, s += i);
            return s;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("0089: for loop with multiple comma expressions") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int i, j, sum = 0;
            for (i = 0, j = 10; i < 5; i++, j--) {
                sum += i + j;
            }
            return sum;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("0089: comma in for condition") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int i = 0;
            int x = 0;
            for (i = 0; i < 3; i++, x = i * 2);
            return x;
        }
    )");
    REQUIRE(result.success);
}
