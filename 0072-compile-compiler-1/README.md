# Lesson 0072: Self-Hosting Milestone 1 — Compile the Compiler

## Status: ⚠️ Partial | Phase: Self-Hosting | Effort: Hard

## Objective

These lessons (0072, 0073, 0074) represent intermediate steps toward
full self-hosting (lesson 0075). They are placeholders in the
self-hosting roadmap.

| Lesson | Step | Status |
|--------|------|--------|
| 0072 | Compile the compiler's own AST definitions | ❌ Not done |
| 0073 | Compile the compiler's own parser | ❌ Not done |
| 0074 | Compile the compiler's own codegen | ❌ Not done |

## Limitation

The compiler is written in C++17 and uses the C++ standard library
(vectors, unique_ptr, strings). It cannot be compiled by a C-only
compiler. Reaching these milestones requires first porting the
compiler to C (see lesson 0071).

## Roadmap

```
Phase 1: This Project (✅ Complete)
  → C++ compiler that emits x86-64 assembly

Phase 2: Port to C (❌ Not Started)
  → Rewrite simplecc in C11 using minimal stdlib
  → Result: c_simplecc that emits x86-64 assembly

Phase 3: Self-Compilation (Lesson 0072-0074)
  → Use c_simplecc to compile c_simplecc's own source
  → Result: a self-compiled c_simplecc binary

Phase 4: Bootstrap (Lesson 0075)
  → Use c_simplecc (compiled by another compiler) to compile c_simplecc
  → Compare output to the original c_simplecc
  → Result: bootstrapped, verified self-hosting compiler
```

## Current State

The project is at Phase 1. The example program in this directory is
a placeholder — the actual self-hosting work has not been done.

## Example

```c
int main() { return 0; }
```
