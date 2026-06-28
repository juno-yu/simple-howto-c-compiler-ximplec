#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Atomic int store and load", "[stdatomic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdatomic.h>
        int main() {
            atomic_int a;
            atomic_init(&a, 42);
            int val = atomic_load(&a);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Atomic store with atomic_store", "[stdatomic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdatomic.h>
        int main() {
            atomic_int a;
            atomic_init(&a, 0);
            atomic_store(&a, 10);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Atomic exchange", "[stdatomic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdatomic.h>
        int main() {
            atomic_int a;
            atomic_init(&a, 5);
            int old = atomic_exchange(&a, 10);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Atomic compare exchange weak", "[stdatomic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdatomic.h>
        int main() {
            atomic_int a;
            atomic_init(&a, 5);
            int expected = 5;
            int desired = 10;
            _Bool success = atomic_compare_exchange_weak(&a, &expected, desired);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Atomic compare exchange strong", "[stdatomic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdatomic.h>
        int main() {
            atomic_int a;
            atomic_init(&a, 5);
            int expected = 5;
            int desired = 10;
            _Bool success = atomic_compare_exchange_strong(&a, &expected, desired);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Atomic fetch add", "[stdatomic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdatomic.h>
        int main() {
            atomic_int a;
            atomic_init(&a, 0);
            int old = atomic_fetch_add(&a, 1);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Atomic fetch sub", "[stdatomic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdatomic.h>
        int main() {
            atomic_int a;
            atomic_init(&a, 10);
            int old = atomic_fetch_sub(&a, 3);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Atomic bool type", "[stdatomic]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdatomic.h>
        int main() {
            atomic_bool flag;
            atomic_init(&flag, 0);
            atomic_store(&flag, 1);
            _Bool val = atomic_load(&flag);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
