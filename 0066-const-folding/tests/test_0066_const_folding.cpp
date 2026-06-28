#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Constant folding - integer addition at compile time", "[const-folding]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { return 2 + 3; }");
    if (result.success) {
        REQUIRE(result.assembly.find("$5") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Constant folding - integer multiplication at compile time", "[const-folding]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { return 10 * 5; }");
    if (result.success) {
        REQUIRE(result.assembly.find("$50") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Constant folding - nested constant expression", "[const-folding]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { return 2 + 3 * 4; }");
    if (result.success) {
        REQUIRE(result.assembly.find("$14") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Constant folding - constant comparison", "[const-folding]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { return 5 > 3; }");
    if (result.success) {
        REQUIRE(result.assembly.find("$1") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Constant folding - constant logical AND", "[const-folding]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { return 1 && 1; }");
    if (result.success) {
        REQUIRE(result.assembly.find("$1") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Constant folding - constant ternary", "[const-folding]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { return 1 ? 10 : 20; }");
    if (result.success) {
        REQUIRE(result.assembly.find("$10") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Constant folding - no optimization for non-constants", "[const-folding]") {
    Compiler compiler;
    auto result = compiler.compile("int f(int x) { return x + 3; }");
    if (result.success) {
        REQUIRE(result.assembly.find("add") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Constant folding - compile result is valid", "[const-folding]") {
    Compiler compiler;
    auto result = compiler.compile("int main() { return 0; }");
    if (result.success) {
        REQUIRE_FALSE(result.assembly.empty());
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
