# Lesson 0075: Bootstrap — Full Self-Hosting

## Status: ⚠️ Partial | Phase: Self-Hosting | Effort: Hard

## Objective

Achieve full self-hosting: a compiler that can compile its own
source code, and the resulting binary can be used to compile itself
again, producing a bit-identical (or near-identical) compiler
binary.

## Bootstrap Recipe

```
1. c_simplecc_v0  ←── compiled by an existing C compiler (e.g., gcc)
2. c_simplecc_v1  ←── compiled by c_simplecc_v0
3. c_simplecc_v2  ←── compiled by c_simplecc_v1
4. Compare c_simplecc_v1 and c_simplecc_v2 (should be byte-identical)
```

If step 4 succeeds, the compiler is **self-hosting** (also called a
"fixed point").

## Implementation Status

| Step | Status |
|------|--------|
| Port simplecc to C | ❌ (prerequisite) |
| First compilation with external C compiler | ❌ |
| Self-compilation produces working binary | ❌ |
| Two self-compilations match | ❌ |
| Verified end-to-end bootstrap | ❌ |

## Limitation

The current compiler is in C++17. Self-hosting requires:

1. A C11 rewrite of the compiler (lesson 0071).
2. Substantial effort — weeks of work to write a usable C compiler
   in C.
3. Verification that the bootstrap process is deterministic.

## Current State

The example program in this directory is a placeholder. The
self-hosting milestone is **not** achieved.

## Why Self-Hosting Matters

A self-hosting compiler proves:

- The compiler can compile programs as complex as itself.
- The compiler is reasonably complete (handles all features its own
  source needs).
- The language is expressive enough to write a compiler in.
- The implementation is stable enough for long-term maintenance.

Notable self-hosted C compilers: GCC, Clang (LLVM), TCC, chibicc.
