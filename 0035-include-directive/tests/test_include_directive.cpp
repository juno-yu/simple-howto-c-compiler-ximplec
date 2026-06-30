#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Angle bracket include with preprocessor", "[include_directive]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdio.h>
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.error_message.find("stdio.h") != std::string::npos);
}

TEST_CASE("Double quote include with preprocessor", "[include_directive]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include "utils.h"
        int main() {
            return 0;
        }
    )");
    REQUIRE_FALSE(result.success);
    REQUIRE_FALSE(result.error_message.empty());
}

TEST_CASE("Multiple includes with preprocessor", "[include_directive]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include <stdio.h>
        #include <stdlib.h>
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.error_message.find("stdio.h") != std::string::npos);
}

TEST_CASE("Include with other code after", "[include_directive]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #include "myheader.h"
        int main() {
            int x = 42;
            return x;
        }
    )");
    REQUIRE_FALSE(result.success);
    REQUIRE_FALSE(result.error_message.empty());
}

TEST_CASE("Include in function body", "[include_directive]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            #include "inner.h"
            return 0;
        }
    )");
    REQUIRE_FALSE(result.success);
    REQUIRE_FALSE(result.error_message.empty());
}

TEST_CASE("Nested include with ifdef", "[include_directive]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        #ifdef USE_HEADER
        #include "header.h"
        #endif
        int main() {
            return 0;
        }
    )");
    REQUIRE(result.success);
}
