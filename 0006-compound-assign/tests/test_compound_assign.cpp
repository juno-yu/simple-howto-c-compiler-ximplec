#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Compound assignment: += operator") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 10;
            x += 5;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("add %rcx, %rax") != std::string::npos);
    REQUIRE(result.assembly.find("mov %rax, -") != std::string::npos);
}

TEST_CASE("Compound assignment: -= operator") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 10;
            x -= 3;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("sub %rcx, %rax") != std::string::npos);
}

TEST_CASE("Compound assignment: *= operator") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 10;
            x *= 2;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("imul %rcx, %rax") != std::string::npos);
}

TEST_CASE("Compound assignment: /= operator") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 10;
            x /= 5;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("cqo") != std::string::npos);
    REQUIRE(result.assembly.find("idiv %rcx") != std::string::npos);
}

TEST_CASE("Compound assignment: %%= operator") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 10;
            x %= 3;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("cqo") != std::string::npos);
    REQUIRE(result.assembly.find("idiv %rcx") != std::string::npos);
    REQUIRE(result.assembly.find("mov %rdx, %rax") != std::string::npos);
}

TEST_CASE("Compound assignment: &= operator") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 10;
            x &= 12;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("and %rcx, %rax") != std::string::npos);
}

TEST_CASE("Compound assignment: |= operator") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 10;
            x |= 5;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("or %rcx, %rax") != std::string::npos);
}

TEST_CASE("Compound assignment: ^= operator") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 10;
            x ^= 7;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("xor %rcx, %rax") != std::string::npos);
}

TEST_CASE("Compound assignment: <<= operator") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 10;
            x <<= 2;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("shl %cl, %rax") != std::string::npos);
}

TEST_CASE("Compound assignment: >>= operator") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 10;
            x >>= 1;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("shr %cl, %rax") != std::string::npos);
}

TEST_CASE("Compound assignment: chained operators") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 10;
            x += 5;
            x -= 3;
            x *= 2;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("add %rcx, %rax") != std::string::npos);
    REQUIRE(result.assembly.find("sub %rcx, %rax") != std::string::npos);
    REQUIRE(result.assembly.find("imul %rcx, %rax") != std::string::npos);
}

TEST_CASE("Compound assignment: with function parameters") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int add_and_double(int a, int b) {
            a += b;
            a *= 2;
            return a;
        }
        int main() {
            return add_and_double(3, 7);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("add_and_double:") != std::string::npos);
}

TEST_CASE("Compound assignment: complex lvalue expression") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int a[3] = {1, 2, 3};
            a[1] *= 10;
            return a[1];
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("imul") != std::string::npos);
}
