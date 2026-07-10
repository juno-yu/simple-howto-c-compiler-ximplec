#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
using namespace simplecc;

TEST_CASE("0090: anonymous enum") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            enum {A=10, B=20, C=30};
            return B;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("0090: anonymous enum with default values") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            enum {X, Y, Z};
            return Y;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("0090: anonymous enum in expression") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            enum {ONE=1, TWO=2, THREE=3};
            int sum = ONE + TWO + THREE;
            return sum;
        }
    )");
    REQUIRE(result.success);
}
