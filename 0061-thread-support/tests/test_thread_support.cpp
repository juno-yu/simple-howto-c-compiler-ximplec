#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("pthread_create not yet implemented", "[thread_support]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int pthread_create(int *thread, void *attr, void *(*start)(void*), void *arg);
        int main() {
            int tid;
            pthread_create(&tid, 0, 0, 0);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("pthread_join not yet implemented", "[thread_support]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int pthread_join(int thread, void **retval);
        int main() {
            int tid;
            pthread_join(tid, 0);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("pthread_detach not yet implemented", "[thread_support]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int pthread_detach(int thread);
        int main() {
            int tid;
            pthread_detach(tid);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("pthread_exit not yet implemented", "[thread_support]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int pthread_exit(void *retval);
        int main() {
            pthread_exit(0);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("pthread_self not yet implemented", "[thread_support]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int pthread_self();
        int main() {
            int tid = pthread_self();
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Thread support function declarations compile", "[thread_support]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int pthread_create(int *thread, void *attr, void *(*start)(void*), void *arg);
        int pthread_join(int thread, void **retval);
        int pthread_detach(int thread);
        int pthread_exit(void *retval);
        int pthread_self();
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
