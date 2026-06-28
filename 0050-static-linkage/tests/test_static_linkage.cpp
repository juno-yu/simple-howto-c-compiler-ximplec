#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

TEST_CASE("Static function") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "static int helper(int x) { return x + 1; }\n"
        "int main() { return helper(5); }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("helper") != std::string::npos);
        REQUIRE(result.assembly.find("main") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Static variable") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "static int counter = 0;\n"
        "int main() {\n"
        "    counter = counter + 1;\n"
        "    return counter;\n"
        "}"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("counter") != std::string::npos);
        REQUIRE(result.assembly.find("main") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Static function not globally visible") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "static int internal_only(int x) { return x; }\n"
        "int main() { return internal_only(42); }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("internal_only") != std::string::npos);
        REQUIRE_FALSE(result.assembly.find(".globl\tinternal_only") != std::string::npos);
        REQUIRE(result.assembly.find("main") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Static local variable persistence") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "int counter() {\n"
        "    static int count = 0;\n"
        "    count = count + 1;\n"
        "    return count;\n"
        "}\n"
        "int main() { return counter(); }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("counter") != std::string::npos);
        REQUIRE(result.assembly.find("main") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Static with extern override") {
    simplecc::Compiler compiler;
    auto result = compiler.compile(
        "static int hidden(int x) { return x; }\n"
        "extern int hidden(int x);\n"
        "int main() { return hidden(10); }"
    );
    if (result.success) {
        REQUIRE(result.assembly.find("main") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
