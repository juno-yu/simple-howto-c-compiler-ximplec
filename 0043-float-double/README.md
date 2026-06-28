# Lesson 0043: Float and Double

## Status: ⚠️ Partial | Phase: Float & Advanced | Effort: Hard (12-16h)

## Objective

Implement `float` and `double` types in the type system. Currently parsed and accepted, but stored as integers — no FPU/SSE code generation is performed.

## Implementation Status

| Feature | Status | Notes |
|---------|--------|-------|
| Lex `float` / `double` keywords | ✅ | Recognized by lexer |
| Parse `float` / `double` types | ✅ | Type specifier chain accepts them |
| Type size in `sizeof` | ✅ | `sizeof(float)` = 4, `sizeof(double)` = 8 |
| Float literals (`3.14f`, `3.14`) | ✅ | Tokenized as float literals |
| Hex float literals (`0x1.0p10`) | ✅ | Tokenized |
| Float storage | ⚠️ | Stored in 8 bytes as integer (no FPU) |
| Float arithmetic (`+`, `-`, `*`, `/`) | ❌ | Not supported — bit-level integer operations on float bits |
| Float comparisons (`==`, `<`) | ❌ | Not supported |
| Float return values | ❌ | Returned as integer in `%rax` |
| Float ↔ int conversion | ❌ | Not implemented |
| SSE2 instructions (`movss`, `addss`) | ❌ | Not generated |
| `long double` (80-bit) | ❌ | Not supported |

## Limitation Summary

The compiler has the **type system infrastructure** for `float` and `double` (lex, parse, type-check, sizeof) but does **not generate floating-point arithmetic** at the x86-64 level. Float values are stored in stack slots as if they were 64-bit integers, so `return 3.14` returns the bit pattern of 3.14 as an integer, not the value 3.

A program that simply declares a `float` and stores/loads it (no arithmetic) will compile. A program that performs `float + float` will produce undefined behavior at runtime because the integer add instruction does not produce the correct float sum.

## Future Work

Full float support would require:
1. Allocate 4 or 8 bytes per `float`/`double` (currently always 8).
2. Generate `movss` / `movsd` for loads and stores.
3. Generate `addss` / `addsd` / `subss` / `subsd` / `mulss` / `mulsd` / `divss` / `divsd` for arithmetic.
4. Generate `ucomiss` / `ucomisd` for comparisons.
5. Generate `cvtsi2ss` / `cvtsi2sd` for int→float conversion.
6. Generate `cvttss2si` / `cvttsd2si` for float→int conversion.
7. Pass float args in `%xmm0`–`%xmm7` per System V ABI (currently they go in integer registers).
8. Return float values in `%xmm0` / `%xmm1`.

## Example

```c
int main() {
    float f = 3.14f;     // compiles, stores 3.14 as int bits
    double d = 2.718;    // compiles, stores 2.718 as int bits
    return 0;            // float arithmetic not supported
}
```

## Source Code References

| Component | File | Description |
|-----------|------|-------------|
| Lexer | `src/lexer.cpp` | `float`, `double` keywords, float literal lexing |
| Parser | `src/parser.cpp` (`parse_type_specifier`) | `float`, `double` in type chain |
| Type size | `src/codegen.cpp` (`get_type_size`) | 4 for float, 8 for double |
| Codegen | `src/codegen.cpp` | No SSE instructions emitted |
