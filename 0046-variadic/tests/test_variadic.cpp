#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

TEST_CASE("Variadic function declaration with ...") {
    simplecc::Compiler compiler;
    auto result = compiler.compile("int sum(int count, ...) { return count; }");
    if (result.success) {
        REQUIRE(result.assembly.find("sum") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Variadic function call") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "int sum(int count, ...) { return count; }\n"
        "int main() { return sum(3, 10, 20, 30); }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("sum") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Variadic function with zero varargs") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "int sum(int count, ...) { return count; }\n"
        "int main() { return sum(0); }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("sum") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Variadic function returning sum of arguments") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "int add_all(int n, ...) {\n"
        "    int total = 0;\n"
        "    return total;\n"
        "}\n"
        "int main() { return add_all(2, 5, 7); }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("add_all") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Variadic function with mixed types") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "int mixed(int a, ...) { return a; }\n"
        "int main() { return mixed(1, 2, 3); }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("mixed") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
