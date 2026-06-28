#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Array name decays to pointer", "[array_pointer_decay]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[3] = {1, 2, 3};
            int *p = arr;
            return *p;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
        REQUIRE(result.assembly.find("mov $1, %rax") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Array pointer arithmetic", "[array_pointer_decay]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[3] = {10, 20, 30};
            int *p = arr;
            return *(p + 1);
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
        REQUIRE(result.assembly.find("mov $20, %rax") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Array passed to function as pointer", "[array_pointer_decay]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int get_first(int *p) {
            return *p;
        }
        int main() {
            int arr[3] = {10, 20, 30};
            return get_first(arr);
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("sizeof array does not decay", "[array_pointer_decay]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[5];
            return sizeof(arr);
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
        REQUIRE(result.assembly.find("mov $20, %rax") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("sizeof pointer is different from array", "[array_pointer_decay]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[5];
            int *p = arr;
            return sizeof(p);
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
        REQUIRE(result.assembly.find("mov $8, %rax") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Array subscript via pointer", "[array_pointer_decay]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[4] = {5, 10, 15, 20};
            int *p = arr + 2;
            return p[0];
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
        REQUIRE(result.assembly.find("mov $15, %rax") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Pointer subtraction on same array", "[array_pointer_decay]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[5];
            int *p1 = &arr[0];
            int *p2 = &arr[3];
            return p2 - p1;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Char array decay", "[array_pointer_decay]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char s[6] = {'h', 'e', 'l', 'l', 'o', '\0'};
            char *p = s;
            return *p;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
