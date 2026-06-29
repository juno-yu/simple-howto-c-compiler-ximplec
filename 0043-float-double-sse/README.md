# Lesson 0043: Float and Double (Real SSE Implementation)

## Status: 🚧 In Progress | Phase: Float & Advanced | Effort: Hard (4-8h)

## Objective

Replace the bit-pattern storage approach from the original `0043-float-double` lesson
with **real SSE arithmetic and conversions**. Floats and doubles are loaded into XMM
registers, operated on with `addss`/`addsd`/`mulss`/`mulsd`/etc., compared with
`ucomiss`/`ucomisd`, and converted to/from integers with `cvtsi2ss`/`cvttss2si`/etc.

The System V x86-64 ABI is followed: float/double arguments go in `%xmm0`–`%xmm7`,
float/double return values go in `%xmm0`.

## Background

The previous lesson (`0043-float-double-bit-pattern`) stored floats as raw 32/64-bit
integer bit patterns in `%rax` and on the stack. The example "worked" only because
the test program used `*(int*)&f` to extract the bit pattern and compared it to the
expected integer value. **It did not implement float arithmetic.**

For example, the bit-pattern lesson would produce this for `float c = a + b`:

```asm
mov $1065353216, %eax    ; 1.0f bit pattern
movl %eax, -8(%rbp)      ; stored as 4-byte int
mov $1073741824, %eax    ; 2.0f bit pattern
movl %eax, -16(%rbp)
movss -16(%rbp), %xmm0   ; accidental SSE load (wasted)
push %rax
movss -8(%rbp), %xmm0    ; accidental SSE load (wasted)
pop %rcx
add %rcx, %rax           ; INTEGER add: gives 0x7F800000 (Inf), not 3.0f!
```

This lesson replaces that with proper SSE.

## Design

### Type-aware Expression Evaluation

The compiler already tracks expression types via `expr_type_stack_` (used for cast
expressions). We extend it so each sub-expression leaves a type tag indicating
whether the result is in `%xmm0` (float/double) or `%rax` (integer/pointer).

| Type Tag   | Result Location | Width    |
|------------|-----------------|----------|
| `int`      | `%rax`          | 8 bytes  |
| `char`     | `%rax`          | 1 byte   |
| `float`    | `%xmm0`         | 4 bytes  |
| `double`   | `%xmm0`         | 8 bytes  |
| `T*`       | `%rax`          | 8 bytes  |

### Float Literal Emission

Float literals are loaded directly into `%xmm0` via bit-pattern transfer:

```c
// float f = 3.14f;
mov $0x4048F5C3, %eax      ; load bit pattern as 32-bit int
movd %eax, %xmm0           ; transfer to SSE register (zero-extend to 64-bit %xmm0)
movss %xmm0, -8(%rbp)      ; store as 4-byte float
```

For doubles, a 64-bit immediate is used:

```c
// double d = 3.14;
movabsq $0x40091EB851EB851F, %rax
movq %rax, %xmm0
movsd %xmm0, -16(%rbp)
```

### Float Loads and Stores

For a local variable, the load width is `movss` (float, 4 bytes) or `movsd`
(double, 8 bytes):

```c
float f = ...;
// load:
movss -8(%rbp), %xmm0      ; result in %xmm0
// store:
movss %xmm0, -8(%rbp)
```

### Binary Operations

The binary expression visitor computes the static type of the result via
`infer_expr_type` (already present for casts). When both operands are
float/double, SSE instructions are used:

```c
// float c = a + b;
movss -8(%rbp), %xmm0      ; load a
movss -16(%rbp), %xmm1     ; load b
addss %xmm1, %xmm0         ; a + b in %xmm0
movss %xmm0, -24(%rbp)     ; store
```

For mixed int + float, the integer is converted to float first via `cvtsi2ss` /
`cvtsi2sd`.

For comparison operators, `ucomiss` / `ucomisd` set `%eflags`, then `setCC` reads
the result and zero-extends to `%rax`.

### Conversions

- `cvtsi2ss %rax, %xmm0` — convert int (in `%rax`) to float (in `%xmm0`)
- `cvtsi2sd %rax, %xmm0` — convert int to double
- `cvttss2si %xmm0, %rax` — convert float to int (truncation)
- `cvttsd2si %xmm0, %rax` — convert double to int
- `cvtss2sd %xmm0, %xmm0` — widen float to double
- `cvtsd2ss %xmm0, %xmm0` — narrow double to float

### Calling Convention (System V x86-64 ABI)

For a function call, arguments are split into integer/pointer and float/double
lists:

```c
double foo(int a, double b, int c) { return a + b + c; }
```

Argument 1: `a` is `int` → goes in `%rdi`
Argument 2: `b` is `double` → goes in `%xmm0`
Argument 3: `c` is `int` → goes in `%rsi`

Return value: `double` → comes back in `%xmm0`

### Return Value Location

For `return expr;` in a function with a float/double return type, the result is
left in `%xmm0` rather than `%rax`. The function epilogue must preserve the XMM
return register.

## Implementation Steps

1. ✅ **Plan** (this document)
2. **Float literals to %xmm0** — modify `FloatLiteralNode::visit`
3. **Float loads/stores** — modify `IdentifierExprNode::visit`, VarDeclNode init,
   AssignExprNode, etc.
4. **SSE arithmetic** — modify `generate_binary` to dispatch on type
5. **SSE comparisons** — same switch with `ucomiss`/`ucomisd` + `setCC`
6. **Conversions** — extend `CastExprNode::visit`
7. **Unary SSE** — negation via `xorps` sign-bit flip or `subss` from 0
8. **Function call ABI** — split args by type, place in integer vs xmm registers
9. **Return value ABI** — use `%xmm0` for float/double returns
10. **Pointer aliasing still works** — `int *p = (int*)&f; *p = 0;` must still work
    (int pointer dereferences use `mov`/`movl`, not SSE)

## What Works

After implementation:

```c
int main() {
    float a = 1.0f, b = 2.0f;
    float c = a + b;            // real addss
    int *p = (int*)&c;
    return *p;                   // returns 0x40400000 = 1077936128
}
```

This now produces real `addss` and returns the correct 3.0f bit pattern (the
bit-pattern lesson also returned the correct bit pattern, but only because it
was computing the literal — it would fail for any computed float).

## What Doesn't Work (Future)

- `long double` (80-bit) — needs x87 FPU or 16-byte SSE
- `_Complex float` / `_Complex double`
- `<math.h>` library functions not inlined (sin/cos/sqrt)
- Variadic functions with float args (always goes through `%al` varargs count)
- Function calls mixing XMM save/restore for caller-saved XMM regs (we let the
  ABI caller-saved convention handle it via the standard prologue)
- `__attribute__((vector_size(N)))` SIMD types
- FMA (fused multiply-add) — would need `vfmadd231ss` etc.

## Test Plan

The new lesson adds tests that:

1. Float arithmetic: `1.0f + 2.0f` returns the correct bit pattern
2. Double arithmetic: `1.5 + 2.5` returns the correct bit pattern
3. Float multiplication: `2.0f * 3.0f` returns 6.0f
4. Float division: `5.0f / 2.0f` returns 2.5f
5. Float comparison: `1.0f < 2.0f` returns 1
6. Float negation: `-1.0f` returns -1.0f
7. Int to float cast: `(float)42` returns 42.0f
8. Float to int cast: `(int)3.7f` returns 3 (truncation)
9. Float args: `double add(double a, double b) { return a+b; } return *(int*)&add(1.5, 2.5);`
10. Float return: `float f() { return 2.0f; } int *p = (int*)&f(); return *p;`
11. Mixed int+float expression: `1.0f + 2` promotes int to float
12. Pointer aliasing still works: `int *p = (int*)&f; *p = 0;`

## References

- [Intel Intrinsics Guide](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html)
- [System V x86-64 ABI](https://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf) § 3.2 (calling convention)
- [GCC: Floating Point Types](https://gcc.gnu.org/onlinedocs/gcc/Floating-Types.html)
- [chibicc: Float](https://github.com/rui314/chibicc/blob/main/codegen.c)
