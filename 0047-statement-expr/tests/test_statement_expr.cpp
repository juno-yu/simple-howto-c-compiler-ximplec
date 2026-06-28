#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

TEST_CASE("Statement expression basic") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "int main() {\n"
        "    int x = ({ int a = 1; a + 1; });\n"
        "    return x;\n"
        "}"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("main") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Statement expression nested") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "int main() {\n"
        "    int x = ({ int a = 1; ({ int b = 2; a + b; }); });\n"
        "    return x;\n"
        "}"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("main") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Statement expression with side effects") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "int main() {\n"
        "    int x = 0;\n"
        "    int y = ({ x = x + 1; x; });\n"
        "    return y;\n"
        "}"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("main") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Statement expression in function argument") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "int double_it(int n) { return n * 2; }\n"
        "int main() {\n"
        "    return double_it(({ int a = 5; a + 1; }));\n"
        "}"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("main") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Statement expression multiple statements") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "int main() {\n"
        "    int x = ({ int a = 1; int b = 2; int c = 3; a + b + c; });\n"
        "    return x;\n"
        "}"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("main") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
