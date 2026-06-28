#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Simple typedef int", "[typedef]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        typedef int MyInt;
        int main() {
            MyInt x = 42;
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $42, %rax") != std::string::npos);
}

TEST_CASE("Typedef usage as variable type", "[typedef]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        typedef int Integer;
        int main() {
            Integer a = 10;
            Integer b = 20;
            return a + b;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $30, %rax") != std::string::npos);
}

TEST_CASE("Typedef char", "[typedef]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        typedef char Byte;
        int main() {
            Byte b = 'A';
            return b;
        }
    )");
    REQUIRE(result.success);
}

TEST_CASE("Typedef in function parameter", "[typedef]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        typedef int Num;
        Num add(Num a, Num b) {
            return a + b;
        }
        int main() {
            return add(3, 4);
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $7, %rax") != std::string::npos);
}

TEST_CASE("Typedef in function return type", "[typedef]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        typedef int Result;
        Result compute() {
            return 99;
        }
        int main() {
            return compute();
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $99, %rax") != std::string::npos);
}

TEST_CASE("Multiple typedefs", "[typedef]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        typedef int Integer;
        typedef char Character;
        int main() {
            Integer x = 10;
            Character c = 'B';
            return x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $10, %rax") != std::string::npos);
}

TEST_CASE("Typedef pointer type", "[typedef]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        typedef int* IntPtr;
        int main() {
            int x = 42;
            IntPtr p = &x;
            return *p;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $42, %rax") != std::string::npos);
}

TEST_CASE("Typedef with struct", "[typedef]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        typedef struct {
            int x;
            int y;
        } Point;
        int main() {
            Point p;
            p.x = 5;
            return p.x;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $5, %rax") != std::string::npos);
}

TEST_CASE("Typedef chain", "[typedef]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        typedef int MyInt;
        typedef MyInt Number;
        int main() {
            Number n = 77;
            return n;
        }
    )");
    REQUIRE(result.success);
    REQUIRE(result.assembly.find("mov $77, %rax") != std::string::npos);
}

TEST_CASE("Typedef bool", "[typedef]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        typedef _Bool Bool;
        int main() {
            Bool b = 1;
            return b;
        }
    )");
    REQUIRE(result.success);
}
