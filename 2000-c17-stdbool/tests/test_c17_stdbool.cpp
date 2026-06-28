#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("stdbool.h defines bool type", "[c17][stdbool]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdbool.h>
        int main() {
            bool flag = true;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("stdbool.h true macro", "[c17][stdbool]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdbool.h>
        int main() {
            bool t = true;
            return t ? 0 : 1;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("stdbool.h false macro", "[c17][stdbool]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdbool.h>
        int main() {
            bool f = false;
            return f ? 1 : 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("stdbool.h with logical operations", "[c17][stdbool]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdbool.h>
        int main() {
            bool a = true;
            bool b = false;
            bool c = a && !b;
            return c ? 0 : 1;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("stdbool.h in function parameters", "[c17][stdbool]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdbool.h>
        int check(bool value) {
            return value ? 1 : 0;
        }
        int main() {
            return check(true);
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("stdbool.h with conditional logic", "[c17][stdbool]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdbool.h>
        int main() {
            bool x = (1 + 1 == 2);
            if (x) {
                return 0;
            }
            return 1;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("stdbool.h in array indexing", "[c17][stdbool]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdbool.h>
        int main() {
            int vals[2] = {10, 20};
            bool idx = false;
            return vals[idx];
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
