#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Sizeof int type", "[sizeof]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return sizeof(int);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $4, %rax") != std::string::npos);
}

TEST_CASE("Sizeof char type", "[sizeof]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return sizeof(char);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $1, %rax") != std::string::npos);
}

TEST_CASE("Sizeof void type", "[sizeof]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return sizeof(void);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $1, %rax") != std::string::npos);
}

TEST_CASE("Sizeof pointer type", "[sizeof]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return sizeof(int*);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $8, %rax") != std::string::npos);
}

TEST_CASE("Sizeof expression form", "[sizeof]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x;
            return sizeof(x);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $8, %rax") != std::string::npos);
}

TEST_CASE("Sizeof in assignment", "[sizeof]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int size = sizeof(int);
            return size;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $4, %rax") != std::string::npos);
}

TEST_CASE("Sizeof const int", "[sizeof]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return sizeof(const int);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $4, %rax") != std::string::npos);
}

TEST_CASE("Sizeof const char", "[sizeof]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return sizeof(const char);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $1, %rax") != std::string::npos);
}
