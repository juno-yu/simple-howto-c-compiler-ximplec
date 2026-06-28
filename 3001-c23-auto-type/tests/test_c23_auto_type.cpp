#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("C23 auto type: int auto inference") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            auto x = 42;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $42, %rax") != std::string::npos);
}

TEST_CASE("C23 auto type: bool auto inference") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            auto b = true;
            return b;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $1, %rax") != std::string::npos);
}

TEST_CASE("C23 auto type: char auto inference") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            auto c = 'A';
            return c;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 auto type: auto in for loop") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int sum = 0;
            for (auto i = 0; i < 10; i++) {
                sum += i;
            }
            return sum;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("sum:") != std::string::npos);
}

TEST_CASE("C23 auto type: auto in while loop") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            auto count = 0;
            while (count < 5) {
                count++;
            }
            return count;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 auto type: auto with comparison") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            auto x = 10;
            auto y = 20;
            return x < y;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 auto type: auto in function parameter") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int add(auto a, auto b) {
            return a + b;
        }
        int main() {
            return add(3, 4);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("add:") != std::string::npos);
}

TEST_CASE("C23 auto type: auto in nested blocks") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            auto outer = 10;
            {
                auto inner = 20;
                return outer + inner;
            }
        }
    )");
    REQUIRE(result.success);
}
