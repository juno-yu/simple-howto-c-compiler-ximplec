#include "catch2/catch_test_macros.hpp"
#include "compiler.h"
#include <string>

using namespace simplecc;

static CompileResult compile_ok(const std::string& code) {
    Compiler compiler;
    auto result = compiler.compile(code);
    REQUIRE(result.success);
    return result;
}

// ──────────────────────────────────────────────────────────────────────
// Global aggregate variables
// ──────────────────────────────────────────────────────────────────────

TEST_CASE("Global array reserves full storage", "[init_fixes][globals]") {
    auto r = compile_ok(R"(
        int g[3];
        int main() { g[1] = 20; return g[1]; }
    )");
    // 3 ints = 12 bytes (previously emitted .zero 4)
    REQUIRE(r.assembly.find(".zero 12") != std::string::npos);
}

TEST_CASE("Global array decays to its address", "[init_fixes][globals]") {
    auto r = compile_ok(R"(
        int g[3];
        int main() { g[1] = 20; return g[1]; }
    )");
    // Indexing must compute the address (lea), not load the first 8 bytes
    // (previously emitted "mov g(%rip), %rax" which segfaulted at runtime).
    REQUIRE(r.assembly.find("lea g(%rip), %rax") != std::string::npos);
    REQUIRE(r.assembly.find("mov g(%rip), %rax") == std::string::npos);
}

TEST_CASE("Global array initializer emits every element", "[init_fixes][globals]") {
    auto r = compile_ok(R"(
        int a[3] = {10, 20, 30};
        int main() { return a[2]; }
    )");
    REQUIRE(r.assembly.find(".long 10") != std::string::npos);
    REQUIRE(r.assembly.find(".long 20") != std::string::npos);
    REQUIRE(r.assembly.find(".long 30") != std::string::npos);
}

TEST_CASE("Global array initializer with designators", "[init_fixes][globals]") {
    auto r = compile_ok(R"(
        int a[4] = {[2] = 9};
        int main() { return a[2]; }
    )");
    // Element 2 holds 9; the other three slots are emitted as 0.
    REQUIRE(r.assembly.find(".long 9") != std::string::npos);
    REQUIRE(r.assembly.find(".long 0") != std::string::npos);
    REQUIRE(r.assembly.find(".zero 16") != std::string::npos);
}

TEST_CASE("Unsized global array infers length from initializer", "[init_fixes][globals]") {
    auto r = compile_ok(R"(
        int g[] = {1, 2, 3, 4};
        int main() { return g[3]; }
    )");
    // 4 ints = 16 bytes
    REQUIRE(r.assembly.find(".zero 16") != std::string::npos);
    REQUIRE(r.assembly.find("lea g(%rip), %rax") != std::string::npos);
}

TEST_CASE("Global struct reserves struct-sized storage", "[init_fixes][globals]") {
    auto r = compile_ok(R"(
        struct P { int x; int y; };
        struct P p;
        int main() { p.y = 5; return p.y; }
    )");
    // 2 ints = 8 bytes (previously emitted .zero 4)
    REQUIRE(r.assembly.find(".zero 8") != std::string::npos);
}

TEST_CASE("Global struct designated initializer emits field values", "[init_fixes][globals]") {
    auto r = compile_ok(R"(
        struct P { int x; int y; };
        struct P p = { .y = 7 };
        int main() { return p.y; }
    )");
    REQUIRE(r.assembly.find(".long 7") != std::string::npos);
    REQUIRE(r.assembly.find(".zero 4") != std::string::npos);  // zero-filled .x
}

TEST_CASE("Global char uses .byte directive", "[init_fixes][globals]") {
    auto r = compile_ok(R"(
        char c = 'A';
        int main() { return c; }
    )");
    REQUIRE(r.assembly.find(".byte 65") != std::string::npos);
}

TEST_CASE("Global long uses .quad directive", "[init_fixes][globals]") {
    auto r = compile_ok(R"(
        long n = 123456789012;
        int main() { return (int)(n / 1000000); }
    )");
    REQUIRE(r.assembly.find(".quad 123456789012") != std::string::npos);
}

TEST_CASE("Global char array from string literal", "[init_fixes][globals]") {
    auto r = compile_ok(R"(
        char msg[] = "hi";
        int main() { return msg[1]; }
    )");
    REQUIRE(r.assembly.find(".asciz \"hi\"") != std::string::npos);
}

// ──────────────────────────────────────────────────────────────────────
// Local unsized arrays (int arr[] = {1,2,3})
// ──────────────────────────────────────────────────────────────────────

TEST_CASE("Unsized local array allocates full storage and decays to lea", "[init_fixes]") {
    auto r = compile_ok(R"(
        int main() {
            int arr[] = {1, 2, 3};
            return arr[1];
        }
    )");
    // The identifier must yield its address (lea), not the value of its
    // first element (previously loaded arr[0]=1 as the base and segfaulted).
    REQUIRE(r.assembly.find("lea -") != std::string::npos);
    REQUIRE(r.assembly.find("movl -16(%rbp), %eax") == std::string::npos);
}

// ──────────────────────────────────────────────────────────────────────
// Struct/array initializers (positional, nested, array-of-struct)
// ──────────────────────────────────────────────────────────────────────

TEST_CASE("Positional struct initializer stores all fields at distinct offsets",
          "[init_fixes][struct_init]") {
    auto r = compile_ok(R"(
        struct P { int x; int y; };
        int main() { struct P p = {10, 20}; return p.x + p.y; }
    )");
    // Two 4-byte stores to different offsets (previously only the first
    // field was emitted, with a stride of the whole struct size).
    size_t first_store = r.assembly.find("movl %eax, -");
    REQUIRE(first_store != std::string::npos);
    size_t second_store = r.assembly.find("movl %eax, -", first_store + 1);
    REQUIRE(second_store != std::string::npos);
    // The two stores must target different offsets.
    REQUIRE(first_store != second_store);
}

TEST_CASE("Array-of-struct designated initializer emits element stores",
          "[init_fixes][struct_init]") {
    auto r = compile_ok(R"(
        struct P { int x; int y; };
        int main() { struct P a[2] = {[1] = {.y = 9}}; return a[1].y; }
    )");
    // The .y store must exist (previously nothing was emitted and the
    // program segfaulted because member access double-dereferenced).
    REQUIRE(r.assembly.find("movl %eax, -12(%rbp)") != std::string::npos);
}

TEST_CASE("Array-of-struct positional initializer", "[init_fixes][struct_init]") {
    auto r = compile_ok(R"(
        struct P { int x; int y; };
        int main() { struct P a[2] = {{1, 2}, {3, 4}}; return a[0].y; }
    )");
    REQUIRE(r.success);
    // Four element stores for the four int slots.
    int stores = 0;
    size_t pos = 0;
    while ((pos = r.assembly.find("movl %eax, -", pos)) != std::string::npos) {
        ++stores;
        ++pos;
    }
    REQUIRE(stores >= 4);
}

TEST_CASE("Anonymous nested struct positional initializer", "[init_fixes][struct_init]") {
    auto r = compile_ok(R"(
        struct A { struct { int x; int y; } inner; };
        int main() { struct A a = {{5, 6}}; return a.inner.x + a.inner.y; }
    )");
    // The anonymous member has a real synthesized layout now; the .x/.y
    // offsets are 0/4 (previously field lookup failed and used offset -1).
    REQUIRE(r.assembly.find("lea -1(%rax)") == std::string::npos);
    size_t s1 = r.assembly.find("movl %eax, -");
    REQUIRE(s1 != std::string::npos);
    REQUIRE(r.assembly.find("movl %eax, -", s1 + 1) != std::string::npos);
}

TEST_CASE("C11 anonymous struct member has correct flattened layout", "[init_fixes][c11]") {
    auto r = compile_ok(R"(
        struct Rect {
            struct { int left; int top; };
            struct { int right; int bottom; };
        };
        int main() { struct Rect r; r.left = 1; r.top = 2; r.right = 3; r.bottom = 4;
                     return r.left + r.top + r.right + r.bottom; }
    )");
    // left/top/right/bottom occupy offsets 0,4,8,12 (previously ALL fields
    // aliased to the bogus offset -1).
    REQUIRE(r.assembly.find("lea -1(%rax)") == std::string::npos);
    REQUIRE(r.assembly.find("lea 12(%rax), %rax") != std::string::npos);
}

TEST_CASE("C11 anonymous union member overlays offset 0", "[init_fixes][c11]") {
    auto r = compile_ok(R"(
        struct S { int tag; union { int i; char c; }; };
        int main() { struct S s; s.tag = 1; s.i = 42; return s.tag + s.i; }
    )");
    REQUIRE(r.assembly.find("lea -1(%rax)") == std::string::npos);
    REQUIRE(r.success);
}

TEST_CASE("Struct with array field has correct layout and access", "[init_fixes][struct_init]") {
    auto r = compile_ok(R"(
        struct P { int x; int y; };
        struct Q { struct P ps[2]; int n; };
        int main() { struct Q q; q.ps[1].y = 7; q.n = 3; return q.ps[1].y + q.n; }
    )");
    // ps spans 16 bytes (2 × 8), so .n must be addressed at offset 16 —
    // previously the array dimension was dropped and .n overlapped ps[1].
    REQUIRE(r.assembly.find("lea 16(%rax), %rax") != std::string::npos);
}

TEST_CASE("Nested struct designated initializer still works", "[init_fixes][struct_init]") {
    auto r = compile_ok(R"(
        struct Inner { int val; };
        struct Outer { struct Inner inner; int extra; };
        int main() { struct Outer o = { .inner = { .val = 5 }, .extra = 10 };
                     return o.inner.val + o.extra; }
    )");
    REQUIRE(r.success);
    REQUIRE(r.assembly.find(".long") == std::string::npos);  // local, not global
    REQUIRE(r.assembly.find("movl %eax, -") != std::string::npos);  // .val = 5 store
}

// ──────────────────────────────────────────────────────────────────────
// Inline assembly parsing
// ──────────────────────────────────────────────────────────────────────

TEST_CASE("asm volatile parses", "[init_fixes][asm]") {
    auto r = compile_ok(R"(
        int main() { asm volatile("nop"); return 0; }
    )");
    // The raw assembly string is emitted into the output.
    REQUIRE(r.assembly.find("nop") != std::string::npos);
}

TEST_CASE("Extended asm with operands parses", "[init_fixes][asm]") {
    auto r = compile_ok(R"(
        int main() { int x = 1; asm("add $1, %0" : "+r"(x) : ); return x; }
    )");
    REQUIRE(r.success);
}

TEST_CASE("Extended asm with input operands and clobbers parses", "[init_fixes][asm]") {
    auto r = compile_ok(R"(
        int main() { int x = 1, y = 2; asm("add %1, %0" : "+r"(x) : "r"(y) : "eax"); return x; }
    )");
    REQUIRE(r.success);
}
