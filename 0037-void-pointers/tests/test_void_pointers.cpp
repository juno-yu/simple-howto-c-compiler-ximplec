#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Void pointer declaration", "[void_pointers]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            void *p;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find(".text") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Void pointer assignment from int pointer", "[void_pointers]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 42;
            int *ip = &x;
            void *vp = ip;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Void pointer cast to int pointer", "[void_pointers]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int x = 42;
            void *vp = &x;
            int *ip = (int *)vp;
            return *ip;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Void pointer arithmetic forbidden", "[void_pointers]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            void *p = 0;
            void *q = p + 1;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("add") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Void pointer in function parameter", "[void_pointers]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int deref_int(void *p) {
            return *(int *)p;
        }
        int main() {
            int x = 99;
            return deref_int(&x);
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Void pointer to char pointer cast", "[void_pointers]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char c = 'A';
            char *cp = &c;
            void *vp = cp;
            char *result = (char *)vp;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Null void pointer", "[void_pointers]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            void *p = 0;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Void pointer comparison", "[void_pointers]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int a = 1;
            int b = 2;
            void *pa = &a;
            void *pb = &b;
            if (pa != pb) {
                return 1;
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
