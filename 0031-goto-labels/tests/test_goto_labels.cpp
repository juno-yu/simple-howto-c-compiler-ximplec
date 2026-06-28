#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Basic goto forward jump", "[goto_labels]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            goto end;
            return 10;
            end:
            return 42;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("jmp") != std::string::npos);
}

TEST_CASE("Goto with label before main", "[goto_labels]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 0;
            goto skip;
            x = 10;
            skip:
            return x;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Goto backward jump in loop", "[goto_labels]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int i = 0;
            loop:
            i = i + 1;
            if (i < 5) goto loop;
            return i;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find(".L") != std::string::npos);
}

TEST_CASE("Multiple labels", "[goto_labels]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            goto end;
            a:
            return 1;
            b:
            return 2;
            end:
            return 42;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Goto skips statements", "[goto_labels]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int a = 1;
            int b = 2;
            goto end;
            a = 100;
            b = 200;
            end:
            return a + b;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("jmp") != std::string::npos);
}

TEST_CASE("Label with code after", "[goto_labels]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return 42;
            my_label:
            return 0;
        }
    )");
    REQUIRE(result.success);
}
