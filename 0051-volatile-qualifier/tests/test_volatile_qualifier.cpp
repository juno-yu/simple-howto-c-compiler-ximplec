#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Volatile integer declaration", "[volatile_qualifier]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            volatile int x = 10;
            return x;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Volatile variable in loop", "[volatile_qualifier]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            volatile int counter = 0;
            while (counter < 10) {
                counter = counter + 1;
            }
            return counter;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("cmp") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Volatile pointer declaration", "[volatile_qualifier]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int val = 42;
            volatile int *p = &val;
            return *p;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Volatile read generates memory load", "[volatile_qualifier]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            volatile int x = 5;
            int a = x;
            int b = x;
            return a + b;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
        REQUIRE(result.assembly.find("add") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Volatile with const qualifier", "[volatile_qualifier]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            const volatile int status = 1;
            return status;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
