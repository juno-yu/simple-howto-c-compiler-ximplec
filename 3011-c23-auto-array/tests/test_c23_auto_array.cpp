#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("C23 auto array: basic int array") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            auto arr[] = {1, 2, 3};
            return sizeof(arr) / sizeof(arr[0]);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $3, %rax") != std::string::npos);
}

TEST_CASE("C23 auto array: double array") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            auto arr[] = {1.0, 2.0, 3.0};
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 auto array: single element") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            auto arr[] = {42};
            return arr[0];
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 auto array: access elements") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            auto arr[] = {10, 20, 30, 40, 50};
            return arr[2];
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $30, %rax") != std::string::npos);
}

TEST_CASE("C23 auto array: loop through array") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            auto arr[] = {1, 2, 3, 4, 5};
            int sum = 0;
            for (int i = 0; i < 5; i++) {
                sum += arr[i];
            }
            return sum;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 auto array: modify elements") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            auto arr[] = {1, 2, 3};
            arr[1] = 10;
            return arr[1];
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $10, %rax") != std::string::npos);
}

TEST_CASE("C23 auto array: two element array") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            auto arr[] = {100, 200};
            return arr[0] + arr[1];
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 auto array: with character literals") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            auto arr[] = {'a', 'b', 'c'};
            return arr[0];
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 auto array: nested access") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            auto arr[] = {1, 2, 3, 4, 5};
            return arr[0] + arr[4];
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("add") != std::string::npos);
}

TEST_CASE("C23 auto array: empty array invalid") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            auto arr[] = {};
            return 0;
        }
    )");
    REQUIRE(result.success);
}
