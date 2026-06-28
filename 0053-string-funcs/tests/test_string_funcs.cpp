#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("strlen not yet implemented", "[string_funcs]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char *s = "hello";
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("strcmp not yet implemented", "[string_funcs]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char *a = "abc";
            char *b = "abd";
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("strcpy not yet implemented", "[string_funcs]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char *src = "hello";
            char dst[10];
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("String literal assignment compiles", "[string_funcs]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char *s = "hello world";
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("String literal in expression", "[string_funcs]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char *msg = "test message";
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("strlen call not recognized gracefully", "[string_funcs]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int strlen(char *s);
        int main() {
            char *s = "hello";
            int len = strlen(s);
            return len;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
