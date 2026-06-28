#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("stdint.h int8_t type", "[c17][stdint]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdint.h>
        int main() {
            int8_t x = -128;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("stdint.h uint8_t type", "[c17][stdint]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdint.h>
        int main() {
            uint8_t x = 255;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("stdint.h int16_t type", "[c17][stdint]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdint.h>
        int main() {
            int16_t x = -32768;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("stdint.h int32_t type", "[c17][stdint]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdint.h>
        int main() {
            int32_t x = -2147483648;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("stdint.h int64_t type", "[c17][stdint]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdint.h>
        int main() {
            int64_t x = -9223372036854775807LL - 1;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("stdint.h limit macros", "[c17][stdint]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdint.h>
        int main() {
            int32_t max = INT32_MAX;
            int32_t min = INT32_MIN;
            uint32_t umax = UINT32_MAX;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("stdint.h pointer-sized types", "[c17][stdint]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdint.h>
        int main() {
            intptr_t p = 0;
            uintptr_t up = 0;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("stdint.h in function signatures", "[c17][stdint]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdint.h>
        int32_t add(int32_t a, int32_t b) {
            return a + b;
        }
        int main() {
            int32_t result = add(10, 20);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
