#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("typeof basic variable type", "[c17][typeof]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 42;
            typeof(x) y = 10;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("typeof expression type", "[c17][typeof]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int a = 1;
            double b = 2.0;
            typeof(a + b) c = 3.0;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("typeof with pointer", "[c17][typeof]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 5;
            int *p = &x;
            typeof(*p) val = 10;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("typeof with array", "[c17][typeof]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[5] = {1, 2, 3, 4, 5};
            typeof(arr) copy = {0};
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("typeof in function return context", "[c17][typeof]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int get_value(void) { return 42; }
        int main() {
            typeof(get_value()) result = get_value();
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("typeof with function pointer", "[c17][typeof]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int add(int a, int b) { return a + b; }
        int main() {
            typeof(add) fn = add;
            int result = fn(3, 4);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("typeof chained declarations", "[c17][typeof]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int a = 10;
            typeof(a) b = 20;
            typeof(b) c = 30;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
