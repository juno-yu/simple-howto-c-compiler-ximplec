#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
using namespace simplecc;

TEST_CASE("0091: typedef function pointer") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            typedef int (*func_t)(int);
            func_t f = 0;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("0091: typedef function pointer with actual function") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int add(int x) { return x + 1; }
        int main() {
            typedef int (*func_t)(int);
            func_t f = add;
            return f(5);
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("0091: multiple typedef function pointers") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int mul(int x, int y) { return x * y; }
        int main() {
            typedef int (*func2_t)(int, int);
            func2_t f = mul;
            return f(3, 4);
        }
    )");
    REQUIRE(result.success);
}
