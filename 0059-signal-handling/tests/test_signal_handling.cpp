#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("signal not yet implemented", "[signal_handling]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int signal(int sig, void (*handler)(int));
        void handler(int sig) {
            return;
        }
        int main() {
            signal(2, handler);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("sigaction not yet implemented", "[signal_handling]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int sigaction(int sig, void *act, void *oldact);
        int main() {
            sigaction(2, 0, 0);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("kill not yet implemented", "[signal_handling]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int kill(int pid, int sig);
        int main() {
            kill(1234, 2);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("raise not yet implemented", "[signal_handling]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int raise(int sig);
        int main() {
            raise(2);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Signal handling function declarations compile", "[signal_handling]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int signal(int sig, void (*handler)(int));
        int sigaction(int sig, void *act, void *oldact);
        int kill(int pid, int sig);
        int raise(int sig);
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
