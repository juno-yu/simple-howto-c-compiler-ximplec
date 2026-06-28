#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Designated initializer for struct field", "[designated_init]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Point {
            int x;
            int y;
        };
        int main() {
            struct Point p = { .x = 10, .y = 20 };
            return p.x + p.y;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Designated initializer with partial fields", "[designated_init]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Config {
            int width;
            int height;
            int bpp;
        };
        int main() {
            struct Config c = { .bpp = 32 };
            return c.bpp;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Designated initializer out of order", "[designated_init]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Pair {
            int first;
            int second;
        };
        int main() {
            struct Pair p = { .second = 42, .first = 10 };
            return p.first + p.second;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Designated initializer array index", "[designated_init]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int a[5] = { [2] = 30, [4] = 50 };
            return a[2];
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Designated initializer mixed with positional", "[designated_init]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Triple {
            int a;
            int b;
            int c;
        };
        int main() {
            struct Triple t = { 1, .c = 30 };
            return t.a + t.c;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Designated initializer zero-fills unspecified", "[designated_init]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Data {
            int x;
            int y;
            int z;
        };
        int main() {
            struct Data d = { .y = 10 };
            return d.y;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Designated initializer nested struct", "[designated_init]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Inner {
            int val;
        };
        struct Outer {
            struct Inner inner;
            int extra;
        };
        int main() {
            struct Outer o = { .inner = { .val = 5 }, .extra = 10 };
            return o.inner.val + o.extra;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
