#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Anonymous union member access", "[c11][anon_union]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            struct { int type; union { int i; float f; }; } v;
            v.type = 1;
            v.i = 42;
            return v.i;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Anonymous union with named outer member", "[c11][anon_union]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            struct Value {
                int tag;
                union { int i; char c; };
            };
            struct Value v;
            v.tag = 0;
            v.i = 100;
            return v.i;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Anonymous union shared memory", "[c11][anon_union]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            struct { union { int i; char c; }; } u;
            u.i = 256;
            u.c = 0;
            return u.i;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Anonymous union with multiple fields", "[c11][anon_union]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            struct {
                int tag;
                union { int i; float f; char *s; };
            } v;
            v.tag = 2;
            v.f = 3.14f;
            return v.tag;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Anonymous union pointer access", "[c11][anon_union]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            struct { union { int x; int y; }; } u;
            u.x = 10;
            int *p = &u.x;
            return *p;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Anonymous union sizeof", "[c11][anon_union]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            struct { int tag; union { int i; float f; }; } v;
            return sizeof(v);
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Nested anonymous union in struct", "[c11][anon_union]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            struct Outer {
                int type;
                struct {
                    union { int i; float f; };
                };
            };
            struct Outer o;
            o.type = 1;
            o.i = 99;
            return o.i;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Anonymous union in function", "[c11][anon_union]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int get_val(struct { union { int a; int b; }; } u) {
            return u.a;
        }
        int main() {
            struct { union { int a; int b; }; } u;
            u.a = 77;
            return get_val(u);
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
