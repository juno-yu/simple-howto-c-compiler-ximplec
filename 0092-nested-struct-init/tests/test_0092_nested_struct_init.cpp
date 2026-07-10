#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
using namespace simplecc;

TEST_CASE("0092: nested struct initialization and access") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct A { struct { int x; } inner; };
        int main() {
            struct A a;
            a.inner.x = 5;
            return a.inner.x;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("0092: deeply nested struct access") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Outer { struct { struct { int val; } mid; } inner; };
        int main() {
            struct Outer o;
            o.inner.mid.val = 42;
            return o.inner.mid.val;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("0092: nested struct with multiple fields") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Point { struct { int x; int y; } coords; };
        int main() {
            struct Point p;
            p.coords.x = 10;
            p.coords.y = 20;
            return p.coords.x + p.coords.y;
        }
    )");
    REQUIRE(result.success);
}
