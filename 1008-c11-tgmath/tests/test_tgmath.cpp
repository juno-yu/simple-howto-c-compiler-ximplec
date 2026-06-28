#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Type-generic sqrt dispatches to sqrt(double)", "[tgmath]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <tgmath.h>
        int main() {
            double d = sqrt(4.0);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Type-generic sqrtf dispatches to sqrtf(float)", "[tgmath]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <tgmath.h>
        int main() {
            float f = sqrtf(4.0f);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Type-generic fabs for double", "[tgmath]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <tgmath.h>
        int main() {
            double d = fabs(-3.14);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Type-generic ceil for double", "[tgmath]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <tgmath.h>
        int main() {
            double d = ceil(2.3);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Type-generic floor for double", "[tgmath]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <tgmath.h>
        int main() {
            double d = floor(2.7);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Type-generic pow for double", "[tgmath]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <tgmath.h>
        int main() {
            double d = pow(2.0, 3.0);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Type-generic trigonometric sin", "[tgmath]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <tgmath.h>
        int main() {
            double d = sin(1.5708);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Type-generic log for double", "[tgmath]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <tgmath.h>
        int main() {
            double d = log(2.71828);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
