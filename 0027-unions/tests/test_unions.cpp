#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Simple union declaration", "[unions]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        union Value {
            int i;
            float f;
        };
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Union member write and read", "[unions]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        union Value {
            int i;
            float f;
        };
        int main() {
            union Value v;
            v.i = 42;
            return v.i;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $42, %rax") != std::string::npos);
}

TEST_CASE("Union size equals largest member", "[unions]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        union Data {
            int i;
            char c;
            int arr[3];
        };
        int main() {
            return sizeof(union Data);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $12, %rax") != std::string::npos);
}

TEST_CASE("Union with different types", "[unions]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        union Mixed {
            int i;
            char c;
        };
        int main() {
            union Mixed m;
            m.c = 'A';
            return m.c;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Union in function parameter", "[unions]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        union Val {
            int i;
            char c;
        };
        int get_int(union Val v) {
            return v.i;
        }
        int main() {
            union Val v;
            v.i = 10;
            return get_int(v);
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Union variable declaration", "[unions]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        union Box {
            int x;
            int y;
        };
        int main() {
            union Box a;
            union Box b;
            a.x = 5;
            b.x = 10;
            return a.x + b.x;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Union with struct member", "[unions]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Point {
            int x;
            int y;
        };
        union Container {
            int i;
            struct Point p;
        };
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Union field overwrite", "[unions]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        union Value {
            int i;
            char c;
        };
        int main() {
            union Value v;
            v.i = 255;
            v.c = 0;
            return v.c;
        }
    )");
    REQUIRE(result.success);
}
