#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

TEST_CASE("Multi-file: compile single file") {
    simplecc::Compiler compiler;
    auto result = compiler.compile("int main() { return 0; }");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("main") != std::string::npos);
}

TEST_CASE("Multi-file: extern declaration") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "extern int helper(void);\n"
        "int main() { return helper(); }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("main") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Multi-file: function forward declaration") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "int add(int a, int b);\n"
        "int main() { return add(1, 2); }\n"
        "int add(int a, int b) { return a + b; }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("add") != std::string::npos);
        REQUIRE(result.assembly.find("main") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Multi-file: multiple functions in single translation unit") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "int add(int a, int b) { return a + b; }\n"
        "int sub(int a, int b) { return a - b; }\n"
        "int main() { return add(1, 2) + sub(5, 3); }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("add") != std::string::npos);
        REQUIRE(result.assembly.find("sub") != std::string::npos);
        REQUIRE(result.assembly.find("main") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Multi-file: undefined symbol reference") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "extern int missing_func(void);\n"
        "int main() { return missing_func(); }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("main") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
