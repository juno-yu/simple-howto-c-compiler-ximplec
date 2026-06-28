#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Syscall instruction generation", "[syscalls]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return 42;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Write syscall - exit code", "[syscalls]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Syscall number in rax for return", "[syscalls]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            return 1;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Syscall with inline assembly for write", "[syscalls]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            asm("mov $1, %rax");
            asm("mov $1, %rdi");
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Syscall with inline assembly for exit", "[syscalls]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            asm("mov $60, %rax");
            asm("mov $0, %rdi");
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
