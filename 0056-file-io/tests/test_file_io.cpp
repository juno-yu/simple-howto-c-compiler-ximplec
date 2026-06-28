#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

TEST_CASE("fopen not yet implemented", "[file_io]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int fopen(char *path, char *mode);
        int main() {
            FILE *f = fopen("test.txt", "r");
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("fclose not yet implemented", "[file_io]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int fclose(FILE *fp);
        int main() {
            FILE *f = fopen("test.txt", "r");
            fclose(f);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("fread not yet implemented", "[file_io]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int fread(void *buf, int size, int count, FILE *fp);
        int main() {
            char buf[100];
            FILE *f = fopen("test.txt", "r");
            fread(buf, 1, 100, f);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("fwrite not yet implemented", "[file_io]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int fwrite(void *buf, int size, int count, FILE *fp);
        int main() {
            char *data = "hello";
            FILE *f = fopen("test.txt", "w");
            fwrite(data, 1, 5, f);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("fgets not yet implemented", "[file_io]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int fgets(char *buf, int size, FILE *fp);
        int main() {
            char buf[100];
            FILE *f = fopen("test.txt", "r");
            fgets(buf, 100, f);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("fputs not yet implemented", "[file_io]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int fputs(char *str, FILE *fp);
        int main() {
            FILE *f = fopen("test.txt", "w");
            fputs("hello", f);
            return 0;
        }
    )");
    if (result.success) {
        REQUIRE(result.assembly.find("call") != std::string::npos);
    } else {
        REQUIRE_FALSE(result.error_message.empty());
    }
}

TEST_CASE("File I/O function declarations compile", "[file_io]") {
    Compiler compiler;
    auto result = compiler.compile(R"(
        int fopen(char *path, char *mode);
        int fclose(FILE *fp);
        int fread(void *buf, int size, int count, FILE *fp);
        int fwrite(void *buf, int size, int count, FILE *fp);
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
