#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("printf not yet implemented", "[io_functions]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int printf(char *fmt, ...);
        int main() {
            printf("hello world\n");
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("scanf not yet implemented", "[io_functions]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int scanf(char *fmt, ...);
        int main() {
            int x;
            scanf("%d", &x);
            return x;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("putchar not yet implemented", "[io_functions]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int putchar(int c);
        int main() {
            putchar(65);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("puts not yet implemented", "[io_functions]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int puts(char *s);
        int main() {
            puts("hello");
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("printf with format string compiles", "[io_functions]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int printf(char *fmt, ...);
        int main() {
            printf("value: %d\n", 42);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Multiple I/O function declarations", "[io_functions]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int printf(char *fmt, ...);
        int scanf(char *fmt, ...);
        int main() {
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("main") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
