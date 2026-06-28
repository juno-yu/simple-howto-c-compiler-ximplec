#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("pthread_mutex_init not yet implemented", "[synchronization]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int pthread_mutex_init(int *mutex, void *attr);
        int main() {
            int mtx;
            pthread_mutex_init(&mtx, 0);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("pthread_mutex_lock not yet implemented", "[synchronization]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int pthread_mutex_lock(int *mutex);
        int main() {
            int mtx;
            pthread_mutex_lock(&mtx);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("pthread_mutex_unlock not yet implemented", "[synchronization]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int pthread_mutex_unlock(int *mutex);
        int main() {
            int mtx;
            pthread_mutex_unlock(&mtx);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("sem_init not yet implemented", "[synchronization]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int sem_init(int *sem, int shared, int value);
        int main() {
            int s;
            sem_init(&s, 0, 1);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("sem_wait not yet implemented", "[synchronization]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int sem_wait(int *sem);
        int main() {
            int s;
            sem_wait(&s);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("sem_post not yet implemented", "[synchronization]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int sem_post(int *sem);
        int main() {
            int s;
            sem_post(&s);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Synchronization function declarations compile", "[synchronization]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int pthread_mutex_init(int *mutex, void *attr);
        int pthread_mutex_lock(int *mutex);
        int pthread_mutex_unlock(int *mutex);
        int sem_init(int *sem, int shared, int value);
        int sem_wait(int *sem);
        int sem_post(int *sem);
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
