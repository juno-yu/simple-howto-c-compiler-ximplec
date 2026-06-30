#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Cast float to int", "[type_casts]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return (int)3.14f;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("cvttss2si") != std::string::npos);
}

TEST_CASE("Cast int to float", "[type_casts]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            float x = (float)42;
            return 0;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("cvtsi2ss") != std::string::npos);
}

TEST_CASE("Cast int to char (truncation)", "[type_casts]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return (char)65;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Cast char to int (widening)", "[type_casts]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char c = 65;
            return (int)c;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Cast int to long (sign extension)", "[type_casts]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = -1;
            long y = (long)x;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Cast double to int", "[type_casts]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return (int)2.7;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("cvttsd2si") != std::string::npos);
}

TEST_CASE("Cast int to double", "[type_casts]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            double d = (double)10;
            return 0;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("cvtsi2sd") != std::string::npos);
}

TEST_CASE("Nested casts", "[type_casts]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return (int)(float)42;
        }
    )");
    REQUIRE(result.success);
}
