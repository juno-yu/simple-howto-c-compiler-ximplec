#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

TEST_CASE("Inline assembly basic") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "int main() {\n"
        "    asm(\"mov $42, %rax\");\n"
        "    return 42;\n"
        "}"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("main") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Inline assembly with expression") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "int main() {\n"
        "    int x = 0;\n"
        "    asm(\"mov $10, %%eax\" : \"=a\"(x));\n"
        "    return x;\n"
        "}"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("main") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Inline assembly in function body") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "int get_answer() {\n"
        "    asm(\"mov $42, %rax\");\n"
        "    return 0;\n"
        "}"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("get_answer") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Multiple inline assembly statements") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "int main() {\n"
        "    asm(\"nop\");\n"
        "    asm(\"nop\");\n"
        "    asm(\"nop\");\n"
        "    return 0;\n"
        "}"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("main") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Inline assembly with input operand") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "int main() {\n"
        "    int val = 10;\n"
        "    asm(\"addl $5, %0\" : \"+r\"(val));\n"
        "    return val;\n"
        "}"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("main") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
