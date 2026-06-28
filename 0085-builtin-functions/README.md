# Lesson 0085: Built-in Functions (`__builtin_*`)

## Status: ✅ Complete | Phase: GCC Extensions | Effort: Easy

## Objective

Recognize and lower the most common GCC built-in functions. Builtins are functions the compiler recognizes and replaces with optimized code or special lowering — they are not real functions and do not require declarations.

## Supported Builtins

| Builtin | Status | Lowering |
|---------|--------|----------|
| `__builtin_expect(expr, expected)` | ✅ | Returns `expr` unchanged (hint to branch predictor) |
| `__builtin_offsetof(type, member)` | ✅ | Computes byte offset of `member` in `type` at compile time |
| `__builtin_popcount(x)` | ⚠️ | Emitted as call to external `popcount` symbol (must be linked) |
| `__builtin_popcountll(x)` | ⚠️ | Same as above |
| `__builtin_va_list`, `__builtin_va_start`, etc. | ❌ | Not implemented; variadic args unsupported |

## Implementation

The codegen's `visitCallExprNode` checks the callee name:
- `__builtin_expect` → emit code to evaluate the first argument and put it in `%rax`.
- `__builtin_offsetof` → use the `get_field_offset()` helper to compute the offset; emit as integer literal.
- `__builtin_popcount` → emit `call popcount` (or `popcountl` for `long`).

## Limitations

- **No actual branch prediction** — `__builtin_expect` is lowered to its first argument; the optimizer would normally reorder the comparison, but we have no such pass.
- **`__builtin_popcount` is not inlined** — the program must be linked with a function called `popcount`. (GCC inlines it to `popcnt` instruction on supported CPUs.)
- **Many builtins not implemented** — `__builtin_constant_p`, `__builtin_types_compatible_p`, `__builtin_trap`, `__builtin_memcpy`, etc.

## Example

```c
int main() {
    int x = __builtin_expect(42, 1);   // returns 42
    int y = __builtin_popcount(255);    // emitted as: call popcount; result 8
    return x + y;                       // returns 50
}
```

## Source Code References

| Component | File | Description |
|-----------|------|-------------|
| Codegen | `src/codegen.cpp` | `visitCallExprNode` detects `__builtin_*` |
| Helper | `src/codegen.cpp` | `get_field_offset()` for `__builtin_offsetof` |
