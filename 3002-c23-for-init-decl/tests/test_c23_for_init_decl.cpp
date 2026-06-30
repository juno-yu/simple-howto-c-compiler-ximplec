#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("C23 for-init-decl: int declaration in for") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int sum = 0;
            for (int i = 0; i < 10; i++) {
                sum += i;
            }
            return sum;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 for-init-decl: auto declaration in for") {
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
}

TEST_CASE("C23 for-init-decl: bool declaration in for") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int count = 0;
            for (bool done = false; !done; ) {
                count++;
                if (count >= 5) done = true;
            }
            return count;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 for-init-decl: multiple statements in for-init") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int sum = 0;
            for (int i = 0, j = 10; i < j; i++, j--) {
                sum++;
            }
            return sum;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 for-init-decl: nested for with declarations") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int total = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    total++;
                }
            }
            return total;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 for-init-decl: empty for-init") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int i = 0;
            for (; i < 10; i++) {
            }
            return i;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 for-init-decl: for-init scope limited") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            for (int i = 0; i < 5; i++) {
            }
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 for-init-decl: complex expression in for-init") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int sum = 0;
            for (int i = 0; i < 10; i += 2) {
                sum += i;
            }
            return sum;
        }
    )");
    REQUIRE(result.success);
}
