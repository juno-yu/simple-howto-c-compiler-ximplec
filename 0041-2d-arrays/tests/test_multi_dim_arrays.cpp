#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Two-dimensional array declaration", "[multi_dim_arrays]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[3][4];
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Two-dimensional array with initializer", "[multi_dim_arrays]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int m[2][3] = {{1, 2, 3}, {4, 5, 6}};
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Two-dimensional array indexing", "[multi_dim_arrays]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int m[2][3] = {{1, 2, 3}, {4, 5, 6}};
            return m[1][1];
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
        REQUIRE(result.assembly.find("mov $5, %rax") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Three-dimensional array declaration", "[multi_dim_arrays]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[2][3][4];
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Multi-dimensional array assignment", "[multi_dim_arrays]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int m[2][3];
            m[0][0] = 10;
            m[1][2] = 42;
            return m[1][2];
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
        REQUIRE(result.assembly.find("mov $42, %rax") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Multi-dimensional array element in expression", "[multi_dim_arrays]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int m[2][3] = {{1, 2, 3}, {4, 5, 6}};
            return m[0][2] + m[1][0];
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
        REQUIRE(result.assembly.find("add") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Multi-dimensional array row-major layout", "[multi_dim_arrays]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int m[3][2] = {{1, 2}, {3, 4}, {5, 6}};
            return m[2][1];
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
        REQUIRE(result.assembly.find("mov $6, %rax") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Multi-dimensional array with variable index", "[multi_dim_arrays]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int m[3][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12}};
            int i = 2;
            int j = 1;
            return m[i][j];
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
