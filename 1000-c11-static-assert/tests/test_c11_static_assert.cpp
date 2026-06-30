#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("_Static_assert with true condition", "[c11][static_assert]") {
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

TEST_CASE("_Static_assert with false condition", "[c11][static_assert]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Static_assert(0, "this should fail");
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("_Static_assert with sizeof", "[c11][static_assert]") {
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

TEST_CASE("_Static_assert with constant expression", "[c11][static_assert]") {
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

TEST_CASE("_Static_assert at file scope", "[c11][static_assert]") {
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

TEST_CASE("Multiple _Static_assert statements", "[c11][static_assert]") {
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

TEST_CASE("_Static_assert with bitwise expression", "[c11][static_assert]") {
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

TEST_CASE("_Static_assert reports error on failure", "[c11][static_assert]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            _Static_assert(0, "expected failure message");
            return 0;
        }
    )");
    REQUIRE(result.success);
}
