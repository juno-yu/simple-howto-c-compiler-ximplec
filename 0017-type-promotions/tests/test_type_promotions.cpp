#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Char promoted to int in arithmetic", "[type_promotions]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char a = 1;
            char b = 2;
            int c = a + b;
            return c;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("movzbl") != std::string::npos);
}

TEST_CASE("Short promoted to int", "[type_promotions]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            short a = 10;
            short b = 20;
            int c = a + b;
            return c;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("movswl") != std::string::npos);
}

TEST_CASE("Int promoted to float in mixed expression", "[type_promotions]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int a = 5;
            float b = 2.0;
            float c = a + b;
            return 0;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("cvtsi2ss") != std::string::npos);
}

TEST_CASE("Float promoted to double in mixed expression", "[type_promotions]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            float a = 1.0;
            double b = 2.0;
            double c = a + b;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Unsigned int with signed int mixing", "[type_promotions]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            unsigned int a = 10;
            int b = 5;
            unsigned int c = a + b;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Char assignment to int without cast", "[type_promotions]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char c = 65;
            int i = c;
            return i;
        }
    )");
    REQUIRE(result.success);
}
