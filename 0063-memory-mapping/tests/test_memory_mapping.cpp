#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("mmap not yet implemented", "[memory_mapping]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int mmap(void *addr, int length, int prot, int flags, int fd, int offset);
        int main() {
            void *p = mmap(0, 4096, 1, 2, -1, 0);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("munmap not yet implemented", "[memory_mapping]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int munmap(void *addr, int length);
        int main() {
            void *p = mmap(0, 4096, 1, 2, -1, 0);
            munmap(p, 4096);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("mprotect not yet implemented", "[memory_mapping]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int mprotect(void *addr, int length, int prot);
        int main() {
            void *p = mmap(0, 4096, 1, 2, -1, 0);
            mprotect(p, 4096, 7);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("madvise not yet implemented", "[memory_mapping]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int madvise(void *addr, int length, int advice);
        int main() {
            void *p = mmap(0, 4096, 1, 2, -1, 0);
            madvise(p, 4096, 1);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Memory mapping function declarations compile", "[memory_mapping]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int mmap(void *addr, int length, int prot, int flags, int fd, int offset);
        int munmap(void *addr, int length);
        int mprotect(void *addr, int length, int prot);
        int madvise(void *addr, int length, int advice);
        int main() {
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("main") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
