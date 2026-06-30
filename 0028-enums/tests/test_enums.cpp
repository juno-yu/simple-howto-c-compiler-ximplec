#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Simple enum declaration", "[enums]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        enum Color { RED, GREEN, BLUE };
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Enum auto-assigned values", "[enums]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        enum Color { RED, GREEN, BLUE };
        int main() {
            return GREEN;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $1, %rax") != std::string::npos);
}

TEST_CASE("Enum first value is zero", "[enums]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        enum Color { RED, GREEN, BLUE };
        int main() {
            return RED;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $0, %rax") != std::string::npos);
}

TEST_CASE("Enum with explicit values", "[enums]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        enum Status { OK = 200, NOT_FOUND = 404, ERROR = 500 };
        int main() {
            return NOT_FOUND;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $404, %rax") != std::string::npos);
}

TEST_CASE("Enum implicit continuation", "[enums]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        enum Levels { A = 10, B, C };
        int main() {
            return C;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $12, %rax") != std::string::npos);
}

TEST_CASE("Enum in switch", "[enums]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        enum Dir { NORTH, SOUTH, EAST, WEST };
        int main() {
            enum Dir d = EAST;
            if (d == EAST) {
                return 42;
            }
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Enum in variable declaration", "[enums]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        enum Bool { FALSE, TRUE };
        int main() {
            enum Bool b = TRUE;
            return b;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $1, %rax") != std::string::npos);
}

TEST_CASE("Enum comparison", "[enums]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        enum Season { SPRING, SUMMER, FALL, WINTER };
        int main() {
            enum Season s = SUMMER;
            if (s == SUMMER) {
                return 1;
            }
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Multiple enums", "[enums]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        enum Day { MON, TUE, WED };
        enum Month { JAN, FEB, MAR };
        int main() {
            return TUE + FEB;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("add") != std::string::npos);
}

TEST_CASE("Enum in array index", "[enums]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        enum Index { FIRST, SECOND, THIRD };
        int main() {
            int arr[3] = {10, 20, 30};
            return arr[SECOND];
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $20, %rax") != std::string::npos);
}
