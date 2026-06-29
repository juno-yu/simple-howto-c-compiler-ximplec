# Lesson 0071: Self-Hosting Preparation

## Status: ⚠️ Partial | Phase: Self-Hosting | Effort: Hard

## Objective

Prepare the compiler to compile itself. The compiler is written in
C++; the self-hosting goal is to write a C compiler in C that can
compile simplecc's source code.

## Implementation Status

| Step | Status |
|------|--------|
| Decide on C subset to target | ✅ (existing C subset) |
| Refactor C++ compiler to be more portable | ❌ Not done |
| Identify C-only features needed by simplecc | ❌ Not done |
| Write bootstrap C compiler (in C) | ❌ Not done |

## What Is There

The compiler is built with `g++ -std=c++17` (`CMakeLists.txt:4`) and
uses STL containers (`std::vector`, `std::map`, `std::unordered_map`,
`std::string`, `std::unique_ptr`, etc.) throughout the source. It is
not yet portable to a C-only compiler.

The existing C subset of `simplecc` is reasonably complete — it
parses the bulk of C99, emits x86-64 GAS assembly, and links against
the system C library. A self-hosted compiler would have to compile
its own C source through this subset.

## Limitation

This lesson is preparatory — the actual work of rewriting the
compiler in C has not been done. The current compiler is in C++ and
uses the C++17 standard library. To make it self-hosting, a
substantial rewrite is required.

## Future Work

1. Rewrite lexer, parser, codegen, and preprocessor in C (C11).
2. Avoid STL containers; use custom hash maps and dynamic arrays.
3. Use only the bundled C standard library subset (or write custom
   helpers).
4. Ensure the resulting C compiler can compile simplecc's C++ source
   (unlikely) — or write a separate C compiler in C.

## Self-Hosting Levels

| Level | Definition | Status |
|-------|------------|--------|
| 1 | Compiler compiles a non-trivial C program correctly | ✅ (this is met) |
| 2 | Compiler compiles itself (C++ → assembly) | ❌ (not attempted) |
| 3 | Compiler compiles its own C port (C → assembly → re-execute compiler) | ❌ (not done) |
| 4 | Bootstrap from source: compile the C port with an existing compiler, then use the result to recompile itself | ❌ (not done) |
