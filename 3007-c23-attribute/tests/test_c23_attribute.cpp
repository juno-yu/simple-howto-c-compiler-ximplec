#include <catch2/catch_test_macros.hpp>
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("C23 attribute: [[nodiscard]] on function") {
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

TEST_CASE("C23 attribute: [[maybe_unused]] on variable") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            [[maybe_unused]] int x = 42;
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 attribute: [[deprecated]] on function") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        [[deprecated]] int old_func() {
            return 1;
        }
        int main() {
            return old_func();
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("old_func:") != std::string::npos);
}

TEST_CASE("C23 attribute: [[noreturn]] on function") {
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

TEST_CASE("C23 attribute: [[fallthrough]] in switch") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 1;
            switch (x) {
                case 1:
                    x = 2;
                    [[fallthrough]];
                case 2:
                    x = 3;
                    break;
            }
            return x;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 attribute: [[likely]] on statement") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 1;
            if (x > 0) [[likely]] {
                return 1;
            }
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 attribute: [[unlikely]] on statement") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 0;
            if (x > 0) [[unlikely]] {
                return 1;
            }
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 attribute: multiple attributes") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        [[nodiscard, deprecated]] int old_important() {
            return 42;
        }
        int main() {
            return old_important();
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("old_important:") != std::string::npos);
}

TEST_CASE("C23 attribute: attribute on struct") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct [[nodiscard]] point {
            int x;
            int y;
        };
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("C23 attribute: attribute on enum") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        enum [[nodiscard]] color {
            RED, GREEN, BLUE
        };
        int main() {
            return RED;
        }
    )");
    REQUIRE(result.success);
}
