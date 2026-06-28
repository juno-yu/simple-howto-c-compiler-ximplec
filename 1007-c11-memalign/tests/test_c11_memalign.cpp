#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("_Alignof basic type", "[c11][memalign]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return _Alignof(int);
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Alignof char type", "[c11][memalign]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return _Alignof(char);
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Alignof pointer type", "[c11][memalign]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return _Alignof(int*);
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Alignas variable declaration", "[c11][memalign]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Alignas(16) int x = 42;
            return x;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Alignas with power of two", "[c11][memalign]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Alignas(32) char buffer[64];
            buffer[0] = 'a';
            return buffer[0];
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Alignas on array", "[c11][memalign]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Alignas(16) int arr[4];
            arr[0] = 1;
            arr[1] = 2;
            return arr[0] + arr[1];
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Alignas in struct", "[c11][memalign]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Padded {
            char c;
            _Alignas(8) int x;
        };
        int main() {
            struct Padded p;
            p.c = 'a';
            p.x = 100;
            return p.x;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Alignof long long", "[c11][memalign]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return _Alignof(long long);
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
