#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("C23 bool literal: true keyword") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            bool b = true;
            return b;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $1, %rax") != std::string::npos);
}

TEST_CASE("C23 bool literal: false keyword") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            bool b = false;
            return b;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $0, %rax") != std::string::npos);
}

TEST_CASE("C23 bool literal: nullptr keyword") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int* p = nullptr;
            return 0;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $0, %rax") != std::string::npos);
}

TEST_CASE("C23 bool literal: true in conditional") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            if (true) return 1;
            return 0;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $1, %rax") != std::string::npos);
}

TEST_CASE("C23 bool literal: false in conditional") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            if (false) return 1;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 bool literal: nullptr comparison") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int* p = nullptr;
            if (p == nullptr) return 1;
            return 0;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("cmp $0, %rax") != std::string::npos);
}

TEST_CASE("C23 bool literal: not nullptr") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int* p = nullptr;
            if (p != nullptr) return 1;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 bool literal: bool as function return") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        bool is_true() {
            return true;
        }
        int main() {
            return is_true();
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("is_true:") != std::string::npos);
}
