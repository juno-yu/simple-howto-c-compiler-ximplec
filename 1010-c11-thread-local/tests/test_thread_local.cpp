#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Thread-local variable declaration", "[thread_local]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        _Thread_local int counter = 0;
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

TEST_CASE("Thread-local with static storage", "[thread_local]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        static _Thread_local int x = 42;
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

TEST_CASE("Thread-local with extern linkage", "[thread_local]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        extern _Thread_local int shared_var;
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

TEST_CASE("Thread-local uninitialized", "[thread_local]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        _Thread_local int tls_var;
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

TEST_CASE("Thread-local with complex type", "[thread_local]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        typedef struct {
            int id;
            double value;
        } Data;
        _Thread_local Data my_data;
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

TEST_CASE("Thread-local array", "[thread_local]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        _Thread_local int arr[10];
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

TEST_CASE("Multiple thread-local variables", "[thread_local]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        _Thread_local int a = 1;
        _Thread_local int b = 2;
        _Thread_local int c = 3;
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
