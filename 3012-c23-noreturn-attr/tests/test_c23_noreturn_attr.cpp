#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("C23 [[noreturn]]: basic noreturn function") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        [[noreturn]] void die() {
            while(1);
        }
        int main() {
            die();
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("die:") != std::string::npos);
}

TEST_CASE("C23 [[noreturn]]: with exit") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        [[noreturn]] void terminate(int code) {
            while(1);
        }
        int main() {
            terminate(1);
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 [[noreturn]]: with abort") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        void abort(void);
        [[noreturn]] void fatal_error() {
            abort();
        }
        int main() {
            fatal_error();
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 [[noreturn]]: in conditional") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        [[noreturn]] void infinite_loop() {
            while(1);
        }
        int main() {
            int x = 1;
            if (x > 0) {
                infinite_loop();
            }
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 [[noreturn]]: multiple noreturn functions") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        [[noreturn]] void func_a() {
            while(1);
        }
        [[noreturn]] void func_b() {
            while(1);
        }
        int main() {
            func_a();
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 [[noreturn]]: with return type void") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        [[noreturn]] void never_returns() {
            while(1);
        }
        int main() {
            never_returns();
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 [[noreturn]]: function declaration") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        [[noreturn]] void external_noreturn(void);
        [[noreturn]] void external_noreturn(void) {
            while(1);
        }
        int main() {
            external_noreturn();
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 [[noreturn]]: in switch default") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        [[noreturn]] void unreachable() {
            while(1);
        }
        int main() {
            int x = 1;
            switch (x) {
                case 1: return 0;
                default: unreachable();
            }
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 [[noreturn]]: with parameter") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        [[noreturn]] void exit_with_code(int code) {
            while(1);
        }
        int main() {
            exit_with_code(42);
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 [[noreturn]]: chained calls") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        [[noreturn]] void step2() {
            while(1);
        }
        [[noreturn]] void step1() {
            step2();
        }
        int main() {
            step1();
        }
    )");
    REQUIRE(result.success);
}
