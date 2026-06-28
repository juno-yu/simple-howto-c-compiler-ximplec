#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("C23 empty structs: empty struct declaration") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct empty {};
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 empty structs: empty union declaration") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        union empty_union {};
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 empty structs: sizeof empty struct") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct empty {};
        int main() {
            return sizeof(struct empty);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $0, %rax") != std::string::npos);
}

TEST_CASE("C23 empty structs: sizeof empty union") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        union empty_union {};
        int main() {
            return sizeof(union empty_union);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $0, %rax") != std::string::npos);
}

TEST_CASE("C23 empty structs: pointer to empty struct") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct empty {};
        int main() {
            struct empty* p;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 empty structs: array of empty structs") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct empty {};
        int main() {
            struct empty arr[10];
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 empty structs: nested empty structs") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct outer {
            struct inner {};
        };
        int main() {
            return sizeof(struct outer);
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 empty structs: struct with only empty members") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct with_empty {
            struct a {};
            union b {};
        };
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}
