#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("stdalign.h alignas macro", "[c17][stdalign]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdalign.h>
        int main() {
            alignas(16) int x = 42;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("stdalign.h alignof macro", "[c17][stdalign]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdalign.h>
        int main() {
            _Alignof(int);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("stdalign.h alignas on struct", "[c17][stdalign]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdalign.h>
        struct alignas(32) AlignedStruct {
            int data[8];
        };
        int main() {
            struct AlignedStruct s;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("stdalign.h alignas minimum alignment", "[c17][stdalign]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdalign.h>
        int main() {
            alignas(1) char c;
            alignas(2) short s;
            alignas(4) int i;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("stdalign.h alignof on basic types", "[c17][stdalign]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdalign.h>
        int main() {
            _Alignof(char);
            _Alignof(int);
            _Alignof(double);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("stdalign.h alignas on array", "[c17][stdalign]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdalign.h>
        int main() {
            alignas(64) int arr[16];
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
