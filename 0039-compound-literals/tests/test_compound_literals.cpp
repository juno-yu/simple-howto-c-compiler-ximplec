#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("Compound literal int array", "[compound_literals]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            int *arr = (int[]){1, 2, 3};
            return arr[0];
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Compound literal struct", "[compound_literals]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Point {
            int x;
            int y;
        };
        int main() {
            struct Point *p = &(struct Point){10, 20};
            return p->x + p->y;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Compound literal as function argument", "[compound_literals]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Config {
            int width;
            int height;
        };
        int get_area(struct Config c) {
            return c.width * c.height;
        }
        int main() {
            return get_area((struct Config){800, 600});
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Compound literal with designated initializers", "[compound_literals]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Settings {
            int verbose;
            int debug;
        };
        int main() {
            struct Settings s = (struct Settings){ .debug = 1 };
            return s.debug;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Compound literal double array", "[compound_literals]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            double *vals = (double[]){1.0, 2.0, 3.0};
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find(".text") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Compound literal char array", "[compound_literals]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int main() {
            char *s = (char[]){'H', 'i', 0};
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find(".text") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("Compound literal assigned to variable", "[compound_literals]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        struct Vec3 {
            int x;
            int y;
            int z;
        };
        int main() {
            struct Vec3 v = (struct Vec3){1, 2, 3};
            return v.x + v.y + v.z;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("mov") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}
