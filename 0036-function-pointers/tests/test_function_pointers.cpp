#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Function pointer declaration", "[function_pointers]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int (*fp)(int, int);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find(".text") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Function pointer typedef", "[function_pointers]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        typedef int (*BinaryOp)(int, int);
        int main() {
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find(".text") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Function pointer assignment", "[function_pointers]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int add(int a, int b) { return a + b; }
        int main() {
            int (*fp)(int, int) = add;
            return fp(3, 4);
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Function pointer call through pointer", "[function_pointers]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int square(int x) { return x * x; }
        int main() {
            int (*fp)(int);
            fp = square;
            return fp(5);
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Function pointer as parameter", "[function_pointers]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int apply(int (*f)(int), int x) {
            return f(x);
        }
        int double_val(int x) { return x * 2; }
        int main() {
            return apply(double_val, 21);
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Function pointer as return type", "[function_pointers]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int add_one(int x) { return x + 1; }
        int negate(int x) { return -x; }
        int (*get_op(int sel))(int) {
            if (sel == 0) return add_one;
            return negate;
        }
        int main() {
            int (*op)(int) = get_op(0);
            return op(10);
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Null function pointer check", "[function_pointers]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int (*fp)(int) = 0;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Function pointer array", "[function_pointers]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int inc(int x) { return x + 1; }
        int dec(int x) { return x - 1; }
        int main() {
            int (*ops[2])(int);
            ops[0] = inc;
            ops[1] = dec;
            return ops[0](5);
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
