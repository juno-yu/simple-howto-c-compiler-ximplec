#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Struct member access with dot operator", "[struct_access]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Point {
            int x;
            int y;
        };
        int main() {
            struct Point p;
            p.x = 10;
            return p.x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $10, %rax") != std::string::npos);
}

TEST_CASE("Struct member write then read", "[struct_access]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Pair {
            int a;
            int b;
        };
        int main() {
            struct Pair p;
            p.a = 5;
            p.b = 10;
            return p.a;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $5, %rax") != std::string::npos);
}

TEST_CASE("Struct second member access", "[struct_access]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Pair {
            int a;
            int b;
        };
        int main() {
            struct Pair p;
            p.a = 1;
            p.b = 42;
            return p.b;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $42, %rax") != std::string::npos);
}

TEST_CASE("Struct pointer access with arrow operator", "[struct_access]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Point {
            int x;
            int y;
        };
        int main() {
            struct Point p;
            struct Point *ptr = &p;
            ptr->x = 20;
            return ptr->x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $20, %rax") != std::string::npos);
}

TEST_CASE("Nested struct member access", "[struct_access]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Inner {
            int value;
        };
        struct Outer {
            struct Inner nested;
            int extra;
        };
        int main() {
            struct Outer o;
            o.nested.value = 7;
            return o.nested.value;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $7, %rax") != std::string::npos);
}

TEST_CASE("Struct assignment", "[struct_access]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Point {
            int x;
            int y;
        };
        int main() {
            struct Point a;
            a.x = 10;
            a.y = 20;
            struct Point b;
            b = a;
            return b.x;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Struct member in expression", "[struct_access]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Val {
            int num;
        };
        int main() {
            struct Val v;
            v.num = 3;
            return v.num + 7;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $10, %rax") != std::string::npos);
}
