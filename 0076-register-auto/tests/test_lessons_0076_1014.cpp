#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "compiler.h"

using namespace simplecc;

static bool compile_code(const std::string& code) {
    Compiler compiler;
    auto result = compiler.compile(code);
    return result.success;
}

TEST_CASE("0076 register auto qualifier", "[lessons][0076]") {
    REQUIRE(compile_code(R"(
        int main() {
            register int x = 42;
            auto int y = 10;
            return x + y;
        }
    )"));
}

TEST_CASE("0077 restrict qualifier", "[lessons][0077]") {
    REQUIRE(compile_code(R"(
        int main() {
            int * restrict p;
            int x = 42;
            p = &x;
            return *p;
        }
    )"));
}

TEST_CASE("0081 standard lib stubs", "[lessons][0081]") {
    REQUIRE(compile_code(R"(
        int main() {
            int x = 42;
            return x;
        }
    )"));
}

TEST_CASE("0082 statement expression", "[lessons][0082]") {
    REQUIRE(compile_code(R"(
        int main() {
            int x = ({
                int a = 5;
                int b = 10;
                a + b;
            });
            return x;
        }
    )"));
}

TEST_CASE("0083 label as value", "[lessons][0083]") {
    REQUIRE(compile_code(R"(
        int main() {
            int x = 0;
            goto *(&&target);
            x = 100;
            target:
            return x;
        }
    )"));
}

TEST_CASE("0084 attribute gcc", "[lessons][0084]") {
    REQUIRE(compile_code(R"(
        int x __attribute__((unused)) = 42;
        int main() {
            return x;
        }
    )"));
}

TEST_CASE("0085 builtin functions", "[lessons][0085]") {
    REQUIRE(compile_code(R"(
        int main() {
            int x = __builtin_expect(42, 1);
            return x;
        }
    )"));
}

TEST_CASE("1000 static assert", "[lessons][1000]") {
    REQUIRE(compile_code(R"(
        _Static_assert(sizeof(int) == 4, "int must be 4 bytes");
        int main() { return 0; }
    )"));
}

TEST_CASE("1001 generic selection", "[lessons][1001]") {
    REQUIRE(compile_code(R"(
        int main() {
            int x = _Generic(1, int: 1, char: 2, default: 0);
            return x;
        }
    )"));
}

TEST_CASE("1002 anonymous structs", "[lessons][1002]") {
    REQUIRE(compile_code(R"(
        struct Outer {
            struct { int left; int top; };
            struct { int right; int bottom; };
        };
        int main() {
            struct Outer o;
            o.right = 100;
            return o.right;
        }
    )"));
}

TEST_CASE("1005 atomic qualifier", "[lessons][1005]") {
    REQUIRE(compile_code(R"(
        _Atomic int counter = 0;
        int main() {
            counter = 42;
            return counter;
        }
    )"));
}

TEST_CASE("1010 thread local", "[lessons][1010]") {
    REQUIRE(compile_code(R"(
        _Thread_local int tls_var = 42;
        int main() {
            return tls_var;
        }
    )"));
}

TEST_CASE("1014 alignas alignof", "[lessons][1014]") {
    REQUIRE(compile_code(R"(
        int main() {
            int x = _Alignof(int);
            return x;
        }
    )"));
}
