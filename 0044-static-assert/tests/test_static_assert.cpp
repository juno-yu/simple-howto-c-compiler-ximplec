#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Static assert with true condition", "[static_assert]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Static_assert(1, "always true");
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Static assert with false condition", "[static_assert]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Static_assert(0, "this should fail");
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Static assert with constant expression", "[static_assert]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Static_assert(1 + 1 == 2, "math works");
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Static assert with sizeof", "[static_assert]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Static_assert(sizeof(int) == 4, "int is 4 bytes");
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Static assert with macro-like expression", "[static_assert]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #define TRUE 1
        int main() {
            _Static_assert(TRUE, "macro works");
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Multiple static asserts", "[static_assert]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Static_assert(1, "first");
            _Static_assert(2 > 1, "second");
            _Static_assert(sizeof(int) > 0, "third");
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Static assert reports error message", "[static_assert]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Static_assert(0, "expected failure message");
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Static assert in global scope", "[static_assert]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        _Static_assert(1, "global assert");
        int main() {
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Static assert with bitwise expression", "[static_assert]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Static_assert((1 << 3) == 8, "bitshift works");
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
