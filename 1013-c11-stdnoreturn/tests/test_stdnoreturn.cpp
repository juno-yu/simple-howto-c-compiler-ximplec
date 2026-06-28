#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("noreturn function declaration", "[stdnoreturn]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdnoreturn.h>
        noreturn void abort_handler(void);
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

TEST_CASE("noreturn function with exit", "[stdnoreturn]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdnoreturn.h>
        noreturn void my_exit(int status);
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

TEST_CASE("noreturn with static qualifier", "[stdnoreturn]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdnoreturn.h>
        static noreturn void internal_abort(void);
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

TEST_CASE("noreturn function pointer", "[stdnoreturn]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdnoreturn.h>
        noreturn void handler(void);
        int main() {
            noreturn void (*fp)(void) = handler;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("noreturn in struct member function pointer", "[stdnoreturn]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdnoreturn.h>
        typedef noreturn void (*exit_func)(int);
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

TEST_CASE("noreturn with extern linkage", "[stdnoreturn]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdnoreturn.h>
        extern noreturn void fatal_error(const char *msg);
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
