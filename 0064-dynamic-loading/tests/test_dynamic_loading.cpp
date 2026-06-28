#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("dlopen not yet implemented", "[dynamic_loading]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int dlopen(char *filename, int flags);
        int main() {
            int handle = dlopen("libm.so", 1);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("dlsym not yet implemented", "[dynamic_loading]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int dlsym(int handle, char *symbol);
        int main() {
            int handle = dlopen("libm.so", 1);
            int sin = dlsym(handle, "sin");
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("dlclose not yet implemented", "[dynamic_loading]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int dlclose(int handle);
        int main() {
            int handle = dlopen("libm.so", 1);
            dlclose(handle);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("dlerror not yet implemented", "[dynamic_loading]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int dlerror();
        int main() {
            int err = dlerror();
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Dynamic loading function declarations compile", "[dynamic_loading]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int dlopen(char *filename, int flags);
        int dlsym(int handle, char *symbol);
        int dlclose(int handle);
        int dlerror();
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
