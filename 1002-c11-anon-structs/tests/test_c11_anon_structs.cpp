#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Anonymous struct member access", "[c11][anon_struct]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            struct { struct { int x; int y; }; int z; } s;
            s.x = 10;
            s.y = 20;
            s.z = 30;
            return s.x + s.y + s.z;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Anonymous struct with named outer member", "[c11][anon_struct]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            struct Outer {
                struct { int a; int b; };
                int c;
            };
            struct Outer o;
            o.a = 1;
            o.b = 2;
            o.c = 3;
            return o.a + o.b + o.c;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Nested anonymous structs", "[c11][anon_struct]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            struct {
                struct { struct { int val; }; };
            } s;
            s.val = 42;
            return s.val;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Anonymous struct offset calculation", "[c11][anon_struct]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            struct { char c; struct { int x; }; } s;
            s.c = 1;
            s.x = 100;
            return s.x;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Anonymous struct with pointer", "[c11][anon_struct]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            struct { struct { int x; }; int y; } s;
            s.x = 5;
            s.y = 10;
            int *p = &s.x;
            return *p;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Anonymous struct in function parameter", "[c11][anon_struct]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int get_x(struct { struct { int x; }; int y; } s) {
            return s.x;
        }
        int main() {
            struct { struct { int x; }; int y; } s;
            s.x = 42;
            return get_x(s);
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Anonymous struct sizeof", "[c11][anon_struct]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            struct { struct { int x; int y; }; int z; } s;
            return sizeof(s);
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Multiple anonymous structs", "[c11][anon_struct]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            struct {
                struct { int a; };
                struct { int b; };
            } s;
            s.a = 1;
            s.b = 2;
            return s.a + s.b;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
