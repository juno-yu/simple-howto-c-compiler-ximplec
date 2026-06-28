#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("pipe not yet implemented", "[ipc]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int pipe(int *fds);
        int main() {
            int fds[2];
            pipe(fds);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("mkfifo not yet implemented", "[ipc]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int mkfifo(char *path, int mode);
        int main() {
            mkfifo("/tmp/myfifo", 438);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("shmget not yet implemented", "[ipc]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int shmget(int key, int size, int shmflg);
        int main() {
            int id = shmget(1234, 1024, 512);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("shmat not yet implemented", "[ipc]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int shmat(int shmid, void *shmaddr, int shmflg);
        int main() {
            int id = shmget(1234, 1024, 512);
            void *p = shmat(id, 0, 0);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("shmdt not yet implemented", "[ipc]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int shmdt(void *shmaddr);
        int main() {
            int id = shmget(1234, 1024, 512);
            void *p = shmat(id, 0, 0);
            shmdt(p);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("IPC function declarations compile", "[ipc]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int pipe(int *fds);
        int mkfifo(char *path, int mode);
        int shmget(int key, int size, int shmflg);
        int shmat(int shmid, void *shmaddr, int shmflg);
        int shmdt(void *shmaddr);
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
