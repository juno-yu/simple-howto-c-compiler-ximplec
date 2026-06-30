#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("C23 predefined macros: __STDC_VERSION__") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            #if __STDC_VERSION__ >= 202311L
                return 1;
            #else
                return 0;
            #endif
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 predefined macros: __STDC__ defined") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            #ifdef __STDC__
                return 1;
            #else
                return 0;
            #endif
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 predefined macros: __STDC_HOSTED__ defined") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            #ifdef __STDC_HOSTED__
                return 1;
            #else
                return 0;
            #endif
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 predefined macros: __FILE__ defined") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            #ifdef __FILE__
                return 1;
            #else
                return 0;
            #endif
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 predefined macros: __LINE__ defined") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            #ifdef __LINE__
                return 1;
            #else
                return 0;
            #endif
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 predefined macros: __DATE__ defined") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            #ifdef __DATE__
                return 1;
            #else
                return 0;
            #endif
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 predefined macros: __TIME__ defined") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            #ifdef __TIME__
                return 1;
            #else
                return 0;
            #endif
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 predefined macros: __STDC_VERSION__ value") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return __STDC_VERSION__;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $202311, %rax") != std::string::npos);
}

TEST_CASE("C23 predefined macros: __VA_OPT__ usage") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #define LOG(fmt, ...) printf(fmt, ##__VA_ARGS__)
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 predefined macros: combined check") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            #if defined(__STDC__) && defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 202311L)
                return 1;
            #else
                return 0;
            #endif
        }
    )");
    REQUIRE(result.success);
}
