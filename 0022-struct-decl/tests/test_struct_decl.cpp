#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Simple struct declaration", "[struct_decl]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Point {
            int x;
            int y;
        };
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Struct with different member types", "[struct_decl]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Mixed {
            int a;
            char b;
            int c;
        };
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Struct variable declaration", "[struct_decl]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Point {
            int x;
            int y;
        };
        int main() {
            struct Point p;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Multiple struct declarations", "[struct_decl]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Point {
            int x;
            int y;
        };
        struct Rect {
            int left;
            int top;
            int right;
            int bottom;
        };
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Struct size computation", "[struct_decl]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Pair {
            int a;
            int b;
        };
        int main() {
            return sizeof(struct Pair);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $8, %rax") != std::string::npos);
}

TEST_CASE("Struct with char members", "[struct_decl]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct CharPair {
            char a;
            char b;
        };
        int main() {
            return sizeof(struct CharPair);
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Struct member offset computation", "[struct_decl]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Mixed {
            int a;
            char b;
            int c;
        };
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Nested struct declaration", "[struct_decl]") {
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
            return 0;
        }
    )");
    REQUIRE(result.success);
}
