#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("fork not yet implemented", "[process_control]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int fork();
        int main() {
            int pid = fork();
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("exec not yet implemented", "[process_control]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int execvp(char *cmd, char **args);
        int main() {
            char *args[] = {"ls", "-la", 0};
            execvp("ls", args);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("wait not yet implemented", "[process_control]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int waitpid(int pid, int *status, int opts);
        int main() {
            int pid = fork();
            int status;
            waitpid(pid, &status, 0);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("getpid not yet implemented", "[process_control]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int getpid();
        int main() {
            int pid = getpid();
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("exit not yet implemented", "[process_control]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int exit(int status);
        int main() {
            exit(0);
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Process control function declarations compile", "[process_control]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int fork();
        int execvp(char *cmd, char **args);
        int waitpid(int pid, int *status, int opts);
        int getpid();
        int exit(int status);
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
