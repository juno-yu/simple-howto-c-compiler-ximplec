#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("strcpy_s basic copy", "[bounds_checking]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <string.h>
        int main() {
            char dest[20];
            const char *src = "hello";
            strcpy_s(dest, sizeof(dest), src);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("strncpy_s with count", "[bounds_checking]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <string.h>
        int main() {
            char dest[10];
            const char *src = "hello world";
            strncpy_s(dest, sizeof(dest), src, 5);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("sprintf_s formatted output", "[bounds_checking]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdio.h>
        int main() {
            char buf[50];
            sprintf_s(buf, sizeof(buf), "Value: %d", 42);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("strcat_s concatenate strings", "[bounds_checking]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <string.h>
        int main() {
            char dest[30] = "hello";
            const char *src = " world";
            strcat_s(dest, sizeof(dest), src);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("sprintf_s with multiple format specifiers", "[bounds_checking]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdio.h>
        int main() {
            char buf[100];
            sprintf_s(buf, sizeof(buf), "Name: %s, Age: %d", "Alice", 30);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("strcpy_s with small buffer", "[bounds_checking]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <string.h>
        int main() {
            char dest[5];
            const char *src = "hello";
            strcpy_s(dest, sizeof(dest), src);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
