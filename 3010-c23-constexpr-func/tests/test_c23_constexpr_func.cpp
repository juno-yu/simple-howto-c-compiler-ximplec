#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("C23 constexpr function: basic return") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        constexpr int double_val(int x) {
            return x * 2;
        }
        int main() {
            return double_val(5);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("double_val:") != std::string::npos);
}

TEST_CASE("C23 constexpr function: factorial") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        constexpr int factorial(int n) {
            if (n <= 1) return 1;
            return n * factorial(n - 1);
        }
        int main() {
            return factorial(5);
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 constexpr function: square") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        constexpr int square(int x) {
            return x * x;
        }
        int main() {
            return square(7);
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 constexpr function: with const argument") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        constexpr int add(int a, int b) {
            return a + b;
        }
        int main() {
            constexpr int x = 10;
            constexpr int y = 20;
            return add(x, y);
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 constexpr function: recursive fibonacci") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        constexpr int fib(int n) {
            if (n <= 1) return n;
            return fib(n - 1) + fib(n - 2);
        }
        int main() {
            return fib(10);
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 constexpr function: abs value") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        constexpr int abs_val(int x) {
            return (x < 0) ? -x : x;
        }
        int main() {
            return abs_val(-42);
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 constexpr function: power") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        constexpr int power(int base, int exp) {
            int result = 1;
            for (int i = 0; i < exp; i++) {
                result *= base;
            }
            return result;
        }
        int main() {
            return power(2, 10);
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 constexpr function: used in array size") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        constexpr int get_size() {
            return 10;
        }
        int main() {
            int arr[10];
            return sizeof(arr) / sizeof(arr[0]);
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 constexpr function: used in switch case") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        constexpr int get_case_val() {
            return 42;
        }
        int main() {
            int x = 42;
            switch (x) {
                case get_case_val(): return 1;
                default: return 0;
            }
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 constexpr function: multiple parameters") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        constexpr int max_val(int a, int b) {
            return (a > b) ? a : b;
        }
        int main() {
            return max_val(10, 20);
        }
    )");
    REQUIRE(result.success);
}
