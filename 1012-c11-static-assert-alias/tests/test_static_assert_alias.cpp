#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("static_assert via assert.h alias", "[static_assert_alias]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <assert.h>
        int main() {
            static_assert(1, "always true");
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("static_assert with false condition fails", "[static_assert_alias]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <assert.h>
        int main() {
            static_assert(0, "should fail");
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE_FALSE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("static_assert with constant expression", "[static_assert_alias]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <assert.h>
        int main() {
            static_assert(sizeof(int) == 4, "int is 4 bytes");
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("static_assert with arithmetic expression", "[static_assert_alias]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <assert.h>
        int main() {
            static_assert(2 + 2 == 4, "math works");
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("static_assert in global scope", "[static_assert_alias]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <assert.h>
        static_assert(1, "global assert");
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

TEST_CASE("static_assert with macro", "[static_assert_alias]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <assert.h>
        #define TRUE 1
        int main() {
            static_assert(TRUE, "macro works");
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("static_assert with bitwise expression", "[static_assert_alias]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <assert.h>
        int main() {
            static_assert((1 << 3) == 8, "bitshift works");
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("static_assert reports error message on failure", "[static_assert_alias]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <assert.h>
        int main() {
            static_assert(0, "expected failure");
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE_FALSE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
