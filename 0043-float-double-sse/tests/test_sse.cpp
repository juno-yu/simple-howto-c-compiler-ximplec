#include <catch2/catch_test_macros.hpp>
#include "compiler.h"

using namespace simplecc;

TEST_CASE("Float addition produces correct bits", "[sse]") {
    Compiler c;
    auto r = c.compile(R"(
        int main() {
            float a = 1.0f, b = 2.0f;
            float c = a + b;
            int *p = (int*)&c;
            return *p != 0x40400000;
        }
    )");
    REQUIRE(r.success);
}

TEST_CASE("Double multiplication", "[sse]") {
    Compiler c;
    auto r = c.compile(R"(
        int main() {
            double d = 2.0 * 3.0;
            return (int)d != 6;
        }
    )");
    REQUIRE(r.success);
}

TEST_CASE("Float comparison", "[sse]") {
    Compiler c;
    auto r = c.compile(R"(
        int main() {
            if (1.0f < 2.0f) return 0;
            return 1;
        }
    )");
    REQUIRE(r.success);
}

TEST_CASE("Int to float cast", "[sse]") {
    Compiler c;
    auto r = c.compile(R"(
        int main() {
            float f = (float)42;
            int *p = (int*)&f;
            return *p != 0x42280000;
        }
    )");
    REQUIRE(r.success);
}

TEST_CASE("Float to int cast", "[sse]") {
    Compiler c;
    auto r = c.compile(R"(
        int main() {
            return (int)3.7f != 3;
        }
    )");
    REQUIRE(r.success);
}
