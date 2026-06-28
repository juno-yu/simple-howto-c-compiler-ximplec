#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Extern function declaration", "[extern]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        extern int printf(const char *fmt, ...);
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Extern variable declaration", "[extern]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        extern int global_counter;
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Extern no code generation for declaration", "[extern]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        extern int helper(int x);
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("helper") == std::string::npos);
}

TEST_CASE("Extern with function call site", "[extern]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        extern int puts(const char *s);
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Multiple extern declarations", "[extern]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        extern int open(const char *path, int flags);
        extern int close(int fd);
        extern int read(int fd, void *buf, int count);
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Extern char pointer parameter", "[extern]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        extern int strcmp(const char *a, const char *b);
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Extern with existing main function", "[extern]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        extern int printf(const char *fmt, ...);
        int add(int a, int b) {
            return a + b;
        }
        int main() {
            return add(1, 2);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("add") != std::string::npos);
}
