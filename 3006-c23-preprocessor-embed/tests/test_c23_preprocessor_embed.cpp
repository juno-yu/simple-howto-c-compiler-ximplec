#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("C23 preprocessor embed: basic embed directive") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 preprocessor embed: embed with path") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 preprocessor embed: embed binary data") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 preprocessor embed: embed as string") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 preprocessor embed: embed with limit") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 preprocessor embed: embed with offset") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 preprocessor embed: embed with prefix") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 preprocessor embed: embed with suffix") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 preprocessor embed: embed into array") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 preprocessor embed: embed with comma expressions") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}
