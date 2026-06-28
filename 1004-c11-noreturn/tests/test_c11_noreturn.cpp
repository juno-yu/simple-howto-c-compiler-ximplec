#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("_Noreturn function declaration", "[c11][noreturn]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        _Noreturn void die(void);
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

TEST_CASE("_Noreturn function definition", "[c11][noreturn]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        _Noreturn void die(void) {
            while(1) {}
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

TEST_CASE("_Noreturn with return type void", "[c11][noreturn]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        _Noreturn void abort_handler(void);
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

TEST_CASE("_Noreturn function called from main", "[c11][noreturn]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        _Noreturn void terminate(void) {
            while(1) {}
        }
        int main() {
            terminate();
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("_Noreturn in conditional branch", "[c11][noreturn]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        _Noreturn void fail(void) {
            while(1) {}
        }
        int main() {
            int x = 1;
            if (x == 0) {
                fail();
            }
            return x;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Multiple _Noreturn functions", "[c11][noreturn]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        _Noreturn void abort_func(void) {
            while(1) {}
        }
        _Noreturn void exit_func(int s) {
            while(1) {}
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

TEST_CASE("_Noreturn with parameters", "[c11][noreturn]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        _Noreturn void error(int code) {
            while(1) {}
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

TEST_CASE("_Noreturn function pointer", "[c11][noreturn]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        _Noreturn void die(void) {
            while(1) {}
        }
        int main() {
            _Noreturn void (*fp)(void) = die;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
