#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Forward declaration of function without body", "[forward_decl]") {
    Compiler compiler;
    auto result = compiler.compile("int foo();");
    REQUIRE(result.success);
}

TEST_CASE("Forward declaration followed by usage", "[forward_decl]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int add(int a, int b);
        int main() {
            return add(1, 2);
        }
        int add(int a, int b) {
            return a + b;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("add") != std::string::npos);
}

TEST_CASE("Forward declaration with different return type", "[forward_decl]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        void print_hello();
        int main() {
            print_hello();
            return 0;
        }
        void print_hello() {
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Forward declaration with parameters", "[forward_decl]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int multiply(int x, int y);
        int main() {
            return multiply(3, 4);
        }
        int multiply(int x, int y) {
            return x * y;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Forward declaration no body produces no assembly", "[forward_decl]") {
    Compiler compiler;
    auto result = compiler.compile("int foo();");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("foo") == std::string::npos);
}

TEST_CASE("Forward declaration then definition", "[forward_decl]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int factorial(int n);
        int main() {
            return factorial(5);
        }
        int factorial(int n) {
            if (n <= 1) return 1;
            return n * factorial(n - 1);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("factorial") != std::string::npos);
}
