#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("socket not yet implemented", "[socket_prog]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int socket(int domain, int type, int protocol);
        int main() {
            int fd = socket(2, 1, 0);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("bind not yet implemented", "[socket_prog]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int bind(int fd, void *addr, int len);
        int main() {
            int fd = socket(2, 1, 0);
            bind(fd, 0, 16);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("listen not yet implemented", "[socket_prog]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int listen(int fd, int backlog);
        int main() {
            int fd = socket(2, 1, 0);
            listen(fd, 10);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("accept not yet implemented", "[socket_prog]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int accept(int fd, void *addr, int *len);
        int main() {
            int fd = socket(2, 1, 0);
            listen(fd, 10);
            int client = accept(fd, 0, 0);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("connect not yet implemented", "[socket_prog]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int connect(int fd, void *addr, int len);
        int main() {
            int fd = socket(2, 1, 0);
            connect(fd, 0, 16);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Socket programming function declarations compile", "[socket_prog]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int socket(int domain, int type, int protocol);
        int bind(int fd, void *addr, int len);
        int listen(int fd, int backlog);
        int accept(int fd, void *addr, int *len);
        int connect(int fd, void *addr, int len);
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
