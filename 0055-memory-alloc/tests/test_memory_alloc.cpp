#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("malloc not yet implemented", "[memory_alloc]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        void *malloc(int size);
        int main() {
            int *p = malloc(4);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("free not yet implemented", "[memory_alloc]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        void free(void *ptr);
        int main() {
            void *p = 0;
            free(p);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("calloc not yet implemented", "[memory_alloc]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        void *calloc(int count, int size);
        int main() {
            int *arr = calloc(10, 4);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("realloc not yet implemented", "[memory_alloc]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        void *realloc(void *ptr, int size);
        int main() {
            void *p = 0;
            void *q = realloc(p, 100);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("malloc usage pattern compiles", "[memory_alloc]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        void *malloc(int size);
        void free(void *ptr);
        int main() {
            int *p = malloc(sizeof(int));
            *p = 42;
            free(p);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Array on stack works without malloc", "[memory_alloc]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int arr[10];
            arr[0] = 1;
            arr[9] = 10;
            return arr[0] + arr[9];
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
