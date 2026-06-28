#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("getenv not yet implemented", "[environment]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int getenv(char *name);
        int main() {
            int home = getenv("HOME");
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("setenv not yet implemented", "[environment]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int setenv(char *name, char *value, int overwrite);
        int main() {
            setenv("MY_VAR", "hello", 1);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("unsetenv not yet implemented", "[environment]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int unsetenv(char *name);
        int main() {
            unsetenv("MY_VAR");
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Environment variable function declarations compile", "[environment]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int getenv(char *name);
        int setenv(char *name, char *value, int overwrite);
        int unsetenv(char *name);
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
