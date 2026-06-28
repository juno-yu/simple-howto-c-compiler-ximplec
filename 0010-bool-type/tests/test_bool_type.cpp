#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Bool type: declaration") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            bool b = 1;
            return b;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $1, %rax") != std::string::npos);
}

TEST_CASE("Bool type: integer to bool conversion") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            bool b = 42;
            return b;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("bool") != std::string::npos);
}

TEST_CASE("Bool type: zero is false") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            bool b = 0;
            return b;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $0, %rax") != std::string::npos);
}

TEST_CASE("Bool type: logical not") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return !0;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("sete %al") != std::string::npos);
}

TEST_CASE("Bool type: logical not on true") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return !1;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("cmp $0, %rax") != std::string::npos);
    REQUIRE(result.assembly.find("sete %al") != std::string::npos);
}

TEST_CASE("Bool type: in conditional") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            bool flag = 1;
            if (flag) return 1;
            return 0;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("cmp $0, %rax") != std::string::npos);
    REQUIRE(result.assembly.find("je ") != std::string::npos);
}

TEST_CASE("Bool type: comparison result") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            bool b = (5 > 3);
            return b;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("setg %al") != std::string::npos);
    REQUIRE(result.assembly.find("movzbq %al, %rax") != std::string::npos);
}

TEST_CASE("Bool type: as function parameter") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int check(bool b) {
            if (b) return 1;
            return 0;
        }
        int main() {
            return check(1);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("check:") != std::string::npos);
}

TEST_CASE("Bool type: size is 1 byte") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return sizeof(bool);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $1, %rax") != std::string::npos);
}
