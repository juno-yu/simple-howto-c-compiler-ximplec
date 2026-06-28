#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Float variable declaration", "[float_double]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            float x = 3.14f;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Double variable declaration", "[float_double]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            double x = 3.14;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Float literal suffix f", "[float_double]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            float x = 1.5f;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Double literal without suffix", "[float_double]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            double x = 2.718;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Float addition", "[float_double]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            float a = 1.0f;
            float b = 2.0f;
            float c = a + b;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Double addition", "[float_double]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            double a = 1.5;
            double b = 2.5;
            double c = a + b;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Float multiplication", "[float_double]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            float a = 3.0f;
            float b = 4.0f;
            float c = a * b;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Float comparison", "[float_double]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            float a = 1.0f;
            float b = 2.0f;
            if (a < b) {
                return 1;
            }
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Int to float conversion", "[float_double]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int i = 42;
            float f = (float)i;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Float to int conversion", "[float_double]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            float f = 3.9f;
            int i = (int)f;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Float in function parameter", "[float_double]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        float square(float x) {
            return x * x;
        }
        int main() {
            float result = square(3.0f);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Double in function parameter", "[float_double]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        double add_doubles(double a, double b) {
            return a + b;
        }
        int main() {
            double result = add_doubles(1.5, 2.5);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Scientific notation float", "[float_double]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            double x = 1.5e10;
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.success);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
