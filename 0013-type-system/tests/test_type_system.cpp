#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Int type size", "[type_system]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Char type size", "[type_system]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char c;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Void type", "[type_system]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        void do_nothing() {
        }
        int main() {
            do_nothing();
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Pointer type", "[type_system]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int *ptr;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Function return type", "[type_system]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int get_value() {
            return 42;
        }
        int main() {
            return get_value();
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $42, %rax") != std::string::npos);
}

TEST_CASE("Multiple types in declarations", "[type_system]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int a;
            char b;
            int *c;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Type used in assignment", "[type_system]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 42;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $42, %rax") != std::string::npos);
}

TEST_CASE("Function with typed parameters", "[type_system]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int add(int a, int b) {
            return a + b;
        }
        int main() {
            return add(1, 2);
        }
    )");
    REQUIRE(result.success);
}
