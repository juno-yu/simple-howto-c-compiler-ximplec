#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
using namespace simplecc;

TEST_CASE("0093: 2D array initialization") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int a[2][3] = {{1,2,3},{4,5,6}};
            return a[1][2];
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("0093: 2D array access all elements") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int a[2][2] = {{10,20},{30,40}};
            int sum = a[0][0] + a[0][1] + a[1][0] + a[1][1];
            return sum;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("0093: 2D array partial initialization") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int a[3][3] = {{1,2},{4,5}};
            return a[1][1];
        }
    )");
    REQUIRE(result.success);
}
