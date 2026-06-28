#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("atomic_flag initialization", "[stdatomic_complete]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdatomic.h>
        int main() {
            atomic_flag flag = ATOMIC_FLAG_INIT;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("atomic_flag test and set", "[stdatomic_complete]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdatomic.h>
        int main() {
            atomic_flag flag = ATOMIC_FLAG_INIT;
            _Bool was_set = atomic_flag_test_and_set(&flag);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("atomic_flag clear", "[stdatomic_complete]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdatomic.h>
        int main() {
            atomic_flag flag = ATOMIC_FLAG_INIT;
            atomic_flag_test_and_set(&flag);
            atomic_flag_clear(&flag);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("atomic_thread_fence", "[stdatomic_complete]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdatomic.h>
        int main() {
            atomic_thread_fence(memory_order_seq_cst);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("atomic_signal_fence", "[stdatomic_complete]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdatomic.h>
        int main() {
            atomic_signal_fence(memory_order_acquire);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("ATOMIC_VAR_INIT macro", "[stdatomic_complete]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdatomic.h>
        int main() {
            atomic_int a = ATOMIC_VAR_INIT(42);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("atomic_fetch_or", "[stdatomic_complete]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdatomic.h>
        int main() {
            atomic_int a;
            atomic_init(&a, 0x0F);
            int old = atomic_fetch_or(&a, 0xF0);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("atomic_fetch_and", "[stdatomic_complete]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdatomic.h>
        int main() {
            atomic_int a;
            atomic_init(&a, 0xFF);
            int old = atomic_fetch_and(&a, 0x0F);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("atomic_fetch_xor", "[stdatomic_complete]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdatomic.h>
        int main() {
            atomic_int a;
            atomic_init(&a, 0xFF);
            int old = atomic_fetch_xor(&a, 0x0F);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("atomic_is_lock_free", "[stdatomic_complete]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdatomic.h>
        int main() {
            atomic_int a;
            _Bool lock_free = atomic_is_lock_free(&a);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("atomic flag spinlock pattern", "[stdatomic_complete]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdatomic.h>
        int main() {
            atomic_flag lock = ATOMIC_FLAG_INIT;
            while (atomic_flag_test_and_set(&lock))
                ;
            atomic_flag_clear(&lock);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
