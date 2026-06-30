#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("C23 [[nodiscard]]: basic nodiscard function") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        [[nodiscard]] int important() {
            return 42;
        }
        int main() {
            return important();
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("important:") != std::string::npos);
}

TEST_CASE("C23 [[nodiscard]]: with error code") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        [[nodiscard]] int critical_operation(void) {
            return 0;
        }
        int main() {
            return critical_operation();
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 [[nodiscard]]: with message") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        [[nodiscard("error code")]] int error_prone(void) {
            return 1;
        }
        int main() {
            return error_prone();
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 [[nodiscard]]: function declaration") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        [[nodiscard]] int external_func(void);
        [[nodiscard]] int external_func(void) {
            return 100;
        }
        int main() {
            return external_func();
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 [[nodiscard]]: with pointer return") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        [[nodiscard]] int *get_pointer(void) {
            static int val = 42;
            return &val;
        }
        int main() {
            int *p = get_pointer();
            return *p;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 [[nodiscard]]: multiple functions") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        [[nodiscard]] int func_a(void) { return 1; }
        [[nodiscard]] int func_b(void) { return 2; }
        int main() {
            return func_a() + func_b();
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 [[nodiscard]]: in struct") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct [[nodiscard]] Error {
            int code;
            char *message;
        };
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 [[nodiscard]]: with void context cast") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        [[nodiscard]] int important(void) {
            return 42;
        }
        int main() {
            (void)important();
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 [[nodiscard]]: with malloc pattern") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        [[nodiscard]] int safe_alloc(int size) {
            return size;
        }
        int main() {
            int p = safe_alloc(100);
            return (p != 0) ? 1 : 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 [[nodiscard]]: with enum") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        enum [[nodiscard]] Status {
            OK = 0,
            ERROR = -1
        };
        int main() {
            return OK;
        }
    )");
    REQUIRE(result.success);
}
