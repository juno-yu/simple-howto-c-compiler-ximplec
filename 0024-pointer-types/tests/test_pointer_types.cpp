#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Pointer declaration", "[pointer_types]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int *p;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Address-of operator", "[pointer_types]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 42;
            int *p = &x;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Dereference operator", "[pointer_types]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 42;
            int *p = &x;
            return *p;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Pointer to pointer", "[pointer_types]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 10;
            int *p = &x;
            int **pp = &p;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Char pointer", "[pointer_types]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char c = 'A';
            char *p = &c;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Pointer dereference in expression", "[pointer_types]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 5;
            int *p = &x;
            return *p + 3;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Pointer size", "[pointer_types]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return sizeof(int*);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $8, %rax") != std::string::npos);
}

TEST_CASE("Pointer comparison", "[pointer_types]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 1;
            int y = 2;
            int *p = &x;
            int *q = &y;
            if (p == q) {
                return 1;
            }
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Pointer write through dereference", "[pointer_types]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 0;
            int *p = &x;
            *p = 99;
            return x;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("NULL pointer", "[pointer_types]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int *p = 0;
            return 0;
        }
    )");
    REQUIRE(result.success);
}
