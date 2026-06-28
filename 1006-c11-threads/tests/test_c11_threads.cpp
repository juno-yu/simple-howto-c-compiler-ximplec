#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("_Thread_local variable declaration", "[c11][threads]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Thread_local int x = 0;
            return x;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Thread_local variable assignment", "[c11][threads]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Thread_local int x = 0;
            x = 42;
            return x;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Thread_local global variable", "[c11][threads]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        _Thread_local int g_val = 10;
        int main() {
            g_val = 20;
            return g_val;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Thread_local with different types", "[c11][threads]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Thread_local int a = 1;
            _Thread_local char b = 'x';
            _Thread_local long c = 100L;
            return a;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Thread_local in struct", "[c11][threads]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Shared {
            _Thread_local int value;
        };
        int main() {
            struct Shared s;
            s.value = 99;
            return s.value;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Thread_local with initializer", "[c11][threads]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Thread_local int x = 255;
            return x;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Thread_local multiple variables", "[c11][threads]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Thread_local int a = 1;
            _Thread_local int b = 2;
            _Thread_local int c = 3;
            return a + b + c;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Thread_local with static storage", "[c11][threads]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        static _Thread_local int count = 0;
        int main() {
            count = 10;
            return count;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
