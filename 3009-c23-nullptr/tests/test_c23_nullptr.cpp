#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("C23 nullptr: basic null pointer assignment") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int *p = nullptr;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 nullptr: nullptr comparison") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int *p = nullptr;
            if (p == nullptr) {
                return 1;
            }
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 nullptr: nullptr not equal to non-null") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 42;
            int *p = &x;
            if (p != nullptr) {
                return 1;
            }
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 nullptr: nullptr in function argument") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        void process(int *ptr) {
            if (ptr == nullptr) {
                return;
            }
        }
        int main() {
            process(nullptr);
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 nullptr: nullptr in conditional") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int *p = nullptr;
            if (p) {
                return 1;
            }
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 nullptr: nullptr with malloc") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        void *malloc(unsigned long);
        int main() {
            void *p = malloc(100);
            if (p != nullptr) {
                return 1;
            }
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 nullptr: nullptr in return statement") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int *get_null() {
            return nullptr;
        }
        int main() {
            int *p = get_null();
            return (p == nullptr) ? 1 : 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 nullptr: nullptr in ternary operator") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int *p = nullptr;
            int val = (p != nullptr) ? *p : 0;
            return val;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 nullptr: nullptr vs NULL") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #define NULL ((void*)0)
        int main() {
            int *p1 = nullptr;
            int *p2 = NULL;
            return (p1 == p2) ? 1 : 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 nullptr: nullptr in array of pointers") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int *arr[3] = {nullptr, nullptr, nullptr};
            return (arr[0] == nullptr) ? 1 : 0;
        }
    )");
    REQUIRE(result.success);
}
