#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Array declaration", "[array_types]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[10];
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Array with initializer", "[array_types]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[3] = {10, 20, 30};
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Array indexing read", "[array_types]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[3] = {10, 20, 30};
            return arr[1];
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $20, %rax") != std::string::npos);
}

TEST_CASE("Array indexing write", "[array_types]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[3];
            arr[0] = 42;
            return arr[0];
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $42, %rax") != std::string::npos);
}

TEST_CASE("Array element in expression", "[array_types]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[3] = {10, 20, 30};
            return arr[0] + arr[2];
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $40, %rax") != std::string::npos);
}

TEST_CASE("Char array", "[array_types]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char arr[5];
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Array size computation", "[array_types]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[5];
            return sizeof(arr);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $20, %rax") != std::string::npos);
}

TEST_CASE("Array with variable index", "[array_types]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[5] = {1, 2, 3, 4, 5};
            int i = 2;
            return arr[i];
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Multi-dimensional array", "[array_types]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int m[3][4];
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Array of structs", "[array_types]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Point {
            int x;
            int y;
        };
        int main() {
            struct Point arr[3];
            return 0;
        }
    )");
    REQUIRE(result.success);
}
