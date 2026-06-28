#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("C23 constexpr: basic integer constant") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        constexpr int x = 42;
        int main() {
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $42, %rax") != std::string::npos);
}

TEST_CASE("C23 constexpr: const expression in array size") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        constexpr int size = 10;
        int arr[size];
        int main() {
            return sizeof(arr) / sizeof(arr[0]);
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 constexpr: const expression in switch case") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        constexpr int MIN = 0;
        constexpr int MAX = 10;
        int main() {
            int x = 5;
            switch (x) {
                case MIN: return 0;
                case MAX: return 1;
                default: return 2;
            }
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 constexpr: multiple constexpr variables") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        constexpr int a = 10;
        constexpr int b = 20;
        constexpr int c = a + b;
        int main() {
            return c;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $30, %rax") != std::string::npos);
}

TEST_CASE("C23 constexpr: compile-time string length") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        constexpr int len = 5;
        int main() {
            return len;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 constexpr: constexpr with floating point") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        constexpr double pi = 3.14159;
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 constexpr: constexpr in for loop bound") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        constexpr int limit = 10;
        int main() {
            int sum = 0;
            for (int i = 0; i < limit; i++) {
                sum += i;
            }
            return sum;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 constexpr: constexpr pointer") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        constexpr int value = 42;
        constexpr int *p = (int *)&value;
        int main() {
            return *p;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 constexpr: constexpr array") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        constexpr int arr[] = {1, 2, 3, 4, 5};
        int main() {
            return arr[2];
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 constexpr: constexpr struct") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Point {
            int x;
            int y;
        };
        constexpr struct Point origin = {0, 0};
        int main() {
            return origin.x + origin.y;
        }
    )");
    REQUIRE(result.success);
}
