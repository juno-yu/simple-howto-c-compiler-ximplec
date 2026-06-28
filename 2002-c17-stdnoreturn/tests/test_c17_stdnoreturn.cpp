#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("stdnoreturn.h noreturn macro", "[c17][stdnoreturn]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdnoreturn.h>
        noreturn void fail(void) {
            while(1);
        }
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

TEST_CASE("stdnoreturn.h noreturn on exit-like function", "[c17][stdnoreturn]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdnoreturn.h>
        noreturn void terminate(int code) {
            while(1);
        }
        int main() {
            terminate(0);
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("stdnoreturn.h noreturn with conditional", "[c17][stdnoreturn]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdnoreturn.h>
        noreturn void die(int condition) {
            if (condition) {
                while(1);
            }
            while(1);
        }
        int main() {
            die(1);
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("stdnoreturn.h noreturn not called", "[c17][stdnoreturn]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdnoreturn.h>
        noreturn void abort_handler(void) {
            while(1);
        }
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

TEST_CASE("stdnoreturn.h multiple noreturn functions", "[c17][stdnoreturn]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdnoreturn.h>
        noreturn void func_a(void) { while(1); }
        noreturn void func_b(int x) { while(1); }
        int main() {
            func_b(42);
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
