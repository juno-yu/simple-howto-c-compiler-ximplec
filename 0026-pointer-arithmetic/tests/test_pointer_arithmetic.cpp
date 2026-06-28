#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Pointer plus integer", "[pointer_arithmetic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[3] = {10, 20, 30};
            int *p = arr;
            return *(p + 1);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $20, %rax") != std::string::npos);
}

TEST_CASE("Pointer minus integer", "[pointer_arithmetic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[3] = {10, 20, 30};
            int *p = arr + 2;
            return *(p - 1);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $20, %rax") != std::string::npos);
}

TEST_CASE("Pointer increment", "[pointer_arithmetic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[3] = {10, 20, 30};
            int *p = arr;
            p++;
            return *p;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $20, %rax") != std::string::npos);
}

TEST_CASE("Pointer decrement", "[pointer_arithmetic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[3] = {10, 20, 30};
            int *p = arr + 2;
            p--;
            return *p;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $20, %rax") != std::string::npos);
}

TEST_CASE("Pointer difference", "[pointer_arithmetic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[5];
            int *p = arr;
            int *q = arr + 3;
            return q - p;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $3, %rax") != std::string::npos);
}

TEST_CASE("Pointer comparison", "[pointer_arithmetic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[3] = {10, 20, 30};
            int *p = arr;
            int *q = arr + 2;
            if (p < q) {
                return 1;
            }
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Pointer plus offset in expression", "[pointer_arithmetic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[4] = {10, 20, 30, 40};
            int *p = arr;
            return *(p + 2) + *(p + 0);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $40, %rax") != std::string::npos);
}

TEST_CASE("Char pointer arithmetic", "[pointer_arithmetic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char str[4] = {'a', 'b', 'c', 'd'};
            char *p = str;
            return *(p + 2);
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Pointer arithmetic with loop", "[pointer_arithmetic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[3] = {10, 20, 30};
            int *p = arr;
            int sum = 0;
            int i = 0;
            while (i < 3) {
                sum = sum + *p;
                p = p + 1;
                i = i + 1;
            }
            return sum;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Pre-increment pointer", "[pointer_arithmetic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[3] = {10, 20, 30};
            int *p = arr;
            return *++p;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $20, %rax") != std::string::npos);
}
