#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Ifdef without preprocessor support", "[conditional_compilation]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #ifdef DEBUG
        int main() { return 1; }
        #else
        int main() { return 0; }
        #endif
    )");
    REQUIRE_FALSE(result.success);
    REQUIRE_FALSE(result.error_message.empty());
}

TEST_CASE("Ifndef without preprocessor support", "[conditional_compilation]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #ifndef RELEASE
        int main() { return 1; }
        #endif
    )");
    REQUIRE_FALSE(result.success);
    REQUIRE_FALSE(result.error_message.empty());
}

TEST_CASE("If defined without preprocessor support", "[conditional_compilation]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #if defined(PLATFORM_LINUX)
        int main() { return 1; }
        #elif defined(PLATFORM_MAC)
        int main() { return 2; }
        #else
        int main() { return 0; }
        #endif
    )");
    REQUIRE_FALSE(result.success);
    REQUIRE_FALSE(result.error_message.empty());
}

TEST_CASE("Nested ifdef without preprocessor support", "[conditional_compilation]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #ifdef A
        #ifdef B
        int main() { return 2; }
        #else
        int main() { return 1; }
        #endif
        #endif
    )");
    REQUIRE_FALSE(result.success);
    REQUIRE_FALSE(result.error_message.empty());
}

TEST_CASE("If with arithmetic expression", "[conditional_compilation]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #if 1 + 1 == 2
        int main() { return 1; }
        #else
        int main() { return 0; }
        #endif
    )");
    REQUIRE_FALSE(result.success);
    REQUIRE_FALSE(result.error_message.empty());
}

TEST_CASE("Pragma once without preprocessor support", "[conditional_compilation]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #pragma once
        int main() { return 0; }
    )");
    REQUIRE_FALSE(result.success);
    REQUIRE_FALSE(result.error_message.empty());
}
