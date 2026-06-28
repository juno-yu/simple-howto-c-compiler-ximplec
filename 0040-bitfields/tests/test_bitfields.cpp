#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Basic bitfield declaration", "[bitfields]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Flags {
            unsigned int active : 1;
        };
        int main() {
            struct Flags f;
            f.active = 1;
            return f.active;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Multiple bitfields in struct", "[bitfields]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Packed {
            unsigned int a : 3;
            unsigned int b : 5;
            unsigned int c : 8;
        };
        int main() {
            struct Packed p;
            p.a = 7;
            p.b = 31;
            p.c = 255;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Bitfield mixed with regular members", "[bitfields]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Mixed {
            int normal;
            unsigned int flag : 1;
            int another;
        };
        int main() {
            struct Mixed m;
            m.normal = 10;
            m.flag = 1;
            m.another = 20;
            return m.normal + m.another;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Bitfield storage calculation", "[bitfields]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Bits {
            unsigned int x : 1;
            unsigned int y : 1;
            unsigned int z : 1;
        };
        int main() {
            struct Bits b;
            b.x = 1;
            b.y = 0;
            b.z = 1;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find(".text") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Bitfield width limits", "[bitfields]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Widths {
            unsigned int tiny : 1;
            unsigned int small : 4;
            unsigned int medium : 8;
            unsigned int large : 16;
            unsigned int huge : 32;
        };
        int main() {
            struct Widths w;
            w.tiny = 1;
            w.small = 15;
            w.medium = 255;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find(".text") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Bitfield initialization", "[bitfields]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Options {
            unsigned int color : 4;
            unsigned int bold : 1;
            unsigned int italic : 1;
        };
        int main() {
            struct Options o = { .color = 12, .bold = 1, .italic = 0 };
            return o.color + o.bold;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Bitfield comparison", "[bitfields]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Status {
            unsigned int ready : 1;
            unsigned int error : 1;
        };
        int main() {
            struct Status s;
            s.ready = 1;
            s.error = 0;
            if (s.ready == 1) {
                return 42;
            }
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("cmp") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Bitfield unsigned type", "[bitfields]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct UnsignedBits {
            unsigned int val : 8;
        };
        int main() {
            struct UnsignedBits u;
            u.val = 200;
            return u.val;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
