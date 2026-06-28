#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Do-while loop: basic structure") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int i = 0;
            do {
                i += 1;
            } while (i < 5);
            return i;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("do_while_start:") != std::string::npos);
    REQUIRE(result.assembly.find("do_while_end:") != std::string::npos);
}

TEST_CASE("Do-while loop: executes at least once") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int i = 10;
            do {
                i = 0;
            } while (i > 0);
            return i;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("jne ") != std::string::npos);
}

TEST_CASE("Do-while loop: with break") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int i = 0;
            do {
                if (i == 3) break;
                i += 1;
            } while (i < 10);
            return i;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("jmp ") != std::string::npos);
    REQUIRE(result.assembly.find("do_while_end") != std::string::npos);
}

TEST_CASE("Do-while loop: with continue") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int i = 0;
            int sum = 0;
            do {
                i += 1;
                if (i == 5) continue;
                sum += i;
            } while (i < 10);
            return sum;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("do_while_start") != std::string::npos);
}

TEST_CASE("Do-while loop: condition evaluation") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 0;
            do {
                x += 1;
            } while (x < 3);
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("cmp $0, %rax") != std::string::npos);
}

TEST_CASE("Do-while loop: with variable initialization") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int count = 0;
            int n = 100;
            do {
                count += 1;
                n /= 2;
            } while (n > 1);
            return count;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("div") != std::string::npos);
}

TEST_CASE("Do-while loop: nested loops") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int i = 0;
            int j;
            do {
                j = 0;
                do {
                    j += 1;
                } while (j < 3);
                i += 1;
            } while (i < 2);
            return i;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("do_while_start:") != std::string::npos);
}

TEST_CASE("Do-while loop: empty body") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 0;
            do {
            } while (x < 0);
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("do_while_start:") != std::string::npos);
}
