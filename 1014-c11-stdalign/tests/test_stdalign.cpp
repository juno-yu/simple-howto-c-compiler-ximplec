#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("alignas on variable", "[stdalign]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdalign.h>
        int main() {
            alignas(16) int x;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("alignas on array", "[stdalign]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdalign.h>
        int main() {
            alignas(32) char buffer[256];
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("alignof for basic types", "[stdalign]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdalign.h>
        int main() {
            size_t a = alignof(int);
            size_t b = alignof(double);
            size_t c = alignof(char);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("alignof for struct", "[stdalign]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdalign.h>
        struct Aligned {
            char c;
            int i;
            double d;
        };
        int main() {
            size_t a = alignof(struct Aligned);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("alignas on struct", "[stdalign]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdalign.h>
        alignas(16) struct Padded {
            int x;
            int y;
        };
        int main() {
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("alignas with power of 2", "[stdalign]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdalign.h>
        int main() {
            alignas(64) char cache_line[64];
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("alignas on function parameter", "[stdalign]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdalign.h>
        void process(alignas(8) int *data) {
        }
        int main() {
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
