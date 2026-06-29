# Lesson 0043: Float and Double (Bit-Pattern Storage, Legacy)

## Status: ⚠️ Legacy | Superseded by `0043-float-double-sse` | Phase: Float & Advanced

> **Note:** This lesson documents the original bit-pattern storage approach. It is
> **superseded by [`0043-float-double-sse`](../0043-float-double-sse/README.md)**
> which implements real SSE arithmetic. The compiler source code now uses the SSE
> approach; this directory is kept for historical reference and the test fixtures
> that exercise bit-level float operations (e.g. `int *p = (int*)&f;`).

## Objective

Store `float` and `double` as 32/64-bit integer bit patterns in stack slots and
integer registers. This allows `sizeof(float)`, `sizeof(double)`, float literal
parsing, and pointer aliasing (`int *p = (int*)&f`) to work, but **does not
implement real float arithmetic**. The SSE lesson adds real `addss`/`ucomiss`/etc.

## Implementation Status

> **All of the following only works for storage and pointer aliasing — not for
> real float arithmetic.** Float/double variables and literals are stored as
> 4/8-byte integers. To get real IEEE 754 results from `+`, `-`, `*`, `/`, etc.,
> see the SSE lesson.

| Feature | Status | Notes |
|---------|--------|-------|
| Lex `float` / `double` keywords | ✅ | Recognized by lexer |
| Parse `float` / `double` types | ✅ | Type specifier chain accepts them |
| `sizeof(float)` | ✅ | Returns 4 |
| `sizeof(double)` | ✅ | Returns 8 |
| Float literals `3.14f` | ✅ | Tokenized with `f` suffix, bit pattern emitted |
| Float literals `3.14` (double) | ✅ | Tokenized, 8-byte bit pattern emitted |
| Hex float literals `0x1.0p10` | ✅ | Tokenized as FLOAT token |
| Float storage (4 bytes for float) | ✅ | Allocated as 4-byte slots on stack |
| Float storage (8 bytes for double) | ✅ | Allocated as 8-byte slots on stack |
| Float assignment from literal | ✅ | Bits stored correctly via `movss` to stack |
| `&float` address-of | ✅ | Works via `lea` |
| `(int*)&f` pointer cast | ✅ | Type-safe deref reads 4 bytes |
| Float arithmetic (`+`, `-`, `*`, `/`) | ❌ | Not supported here — see SSE lesson |
| Float comparisons (`==`, `<`) | ❌ | Not supported here — see SSE lesson |
| Float return values | ❌ | Stored as int bits, not returned via `%xmm0` |
| Float ↔ int conversion (implicit) | ❌ | Not implemented here — see SSE lesson |
| SSE2 instructions (`movss`, `addss`) | ❌ | Not generated here — see SSE lesson |
| `long double` (80-bit) | ❌ | Not supported |
| Float params/returns via `%xmm0`–`%xmm7` | ❌ | System V ABI requires this |
| Implicit int-to-float promotion | ❌ | Not implemented |

## What Works

Float/double storage, address-of, and pointer-casts are fully working. The
following example compiles, runs, and returns `1` (asserting `sz1==4`, `sz2==8`,
and `bits==1078523331` for `f = 3.14f`):

```c
int main() {
    int sz1 = sizeof(float);
    int sz2 = sizeof(double);
    float f;
    int *p = (int*)&f;
    *p = 0;
    f = 3.14f;
    int bits = *p;
    if (sz1 != 4) return 0;
    if (sz2 != 8) return 0;
    if (bits != 1078523331) return 0;
    return 1;
}
```

Run it end-to-end:

```bash
./build/simplecc -S 0043-float-double-bit-pattern/src/example.c -o /tmp/example.s
gcc -o /tmp/example /tmp/example.s
/tmp/example ; echo "exit: $?"       # → exit: 1
```

The float `3.14f` has its IEEE 754 bit pattern stored at the variable's stack
slot. The `(int*)&f` cast produces an `int*` pointing at that slot, and `*p`
reads the 4-byte float bits as an integer. The generated assembly for the load
+ store is:

```asm
movss -24(%rbp), %xmm0        ; load f (4 bytes, SSE width)
lea  -24(%rbp), %rax          ; compute &f
mov  %rax, -32(%rbp)          ; store int* p
mov  $0, %rax                 ; (initial *p = 0, then f = 3.14f)
mov  $1078523331, %eax        ; bit pattern of 3.14f
movd %eax, %xmm0              ; transfer to SSE register
movss %xmm0, -24(%rbp)        ; store back
mov  -32(%rbp), %rax
movl (%rax), %eax             ; *p reads 4 bytes (movl, not mov)
```

**Limitation:** arithmetic, comparison, and ABI-compliant parameter/return
passing still fall back to bit-pattern storage without SSE. The example above
stays in the "storage / address-of / cast" subset, which is fully working.

## What Doesn't Work and Why

### 1. Float arithmetic — SSE instructions needed

The core blocker: x86-64 integer instructions (`addq`, `subq`, etc.) do not
produce correct IEEE 754 results when applied to raw float bit patterns. For
example:

```c
float a = 1.0f;
float b = 2.0f;
float c = a + b;  // WRONG: integer add gives garbage, not 3.0f
```

To fix this, the codegen must:

- Detect when a binary/unary op involves float or double operands
- Emit `movss`/`movsd` to load operands into XMM registers
- Emit `addss`/`addsd`/`subss`/`mulss`/`divss` etc.
- Store result back with `movss`/`movsd`

This requires knowing the operand types at codegen time (a whole-expression
type inference pass) and adding SSE instruction emission to the binary/unary
visitors.

### 2. Float comparisons — SSE comparison needed

```c
float a = 1.0f;
if (a < 2.0f) { ... }
```

Needs `ucomiss`/`ucomisd` to compare floats, then `setb`/`setnbe` etc. to get
the boolean. The current integer `cmp` does not produce correct results for
float bit patterns.

### 3. Float return values — System V ABI mismatch

The System V x86-64 ABI specifies:

- Float/double args go in `%xmm0`–`%xmm7`
- Float/double return values go in `%xmm0`

If a function returns a float, the caller might read from `%xmm0` (empty) and
the callee puts it in `%rax` — mismatch.

### 4. Float ↔ int implicit conversion

C allows implicit conversion between float and int (e.g., `int x = 3.5f;` → 3,
`float y = 42;` → 42.0f). This requires:

- `cvttss2si` / `cvttsd2si` for float→int (truncation)
- `cvtsi2ss` / `cvtsi2sd` for int→float
- Detection of mixed-type expressions to pick the right conversion

### 5. The "double deref" gotcha

When writing `int bits = *(int*)&f`, the assembly originally loaded 8 bytes
(the full rax) instead of 4 bytes. This was fixed by:

- Tracking variable types in `variable_types_` map
- Using `get_type_size()` to determine load width (1/2/4/8 bytes)
- Emitting `movl` (4 bytes) for int-sized loads instead of `mov` (8 bytes)

The current `generate_unary` `DEREF` case peeks at the operand's type (for an
`IdentifierExprNode` or `CastExprNode`) and chooses the matching `movzbl`/
`movzwl`/`movl`/`mov` width.

## How the Bit Pattern Reaches the Stack

The old, pre-SSE approach was to load the bit pattern as a 32/64-bit integer
into `%rax` and `movl`/`mov` it onto the stack. The current source has
**already moved past that**: the `FloatLiteralNode::visit` visitor (see the
SSE lesson) now transfers the bit pattern from `%eax` to `%xmm0` and uses
`movss`/`movsd` to store it on the stack. So the bit-pattern lesson now
"works" only for code that *doesn't* do float arithmetic — `(int*)&f` style
pointer aliasing, `sizeof(float)`, and direct float literal assignment all
still produce the right bit pattern at the right stack slot.

## Future Work

To complete float support, see the SSE lesson
(`0043-float-double-sse/README.md`). It implements, in order:

1. **Type inference for expressions**: when a `BinaryExprNode` has float-typed
   children, mark the expression as float-typed
2. **SSE loads/stores**: `movss` / `movsd` instead of `mov` for float/double
3. **SSE arithmetic**: `addss`/`addsd`/`subss`/`mulss`/`divss`/`sqrtss`
4. **SSE comparisons**: `ucomiss`/`ucomisd` + `setb`/`setnbe`/`sete`
5. **Conversions**: `cvtsi2ss`/`cvtsi2sd`/`cvttss2si`/`cvttsd2si`/`cvtss2sd`/`cvtsd2ss`
6. **ABI compliance**: float args/returns in `%xmm0`–`%xmm7`

## Source Code References

> **Note:** The current codegen implements the SSE approach. The line numbers
> below point to the *current* source layout — the entries marked "old" are the
> sites where the bit-pattern approach used to live before being replaced by
> SSE. The SSE replacements are in
> [`0043-float-double-sse`](../0043-float-double-sse/README.md).

| Component | File | Description |
|-----------|------|-------------|
| Lexer | `src/lexer.cpp:240-325` | Float literal lexing, `f` suffix marker, hex float `0x1.0p10` |
| Parser — type spec | `src/parser.cpp:175-178` | `float` / `double` keyword in type chain |
| Parser — float literal | `src/parser.cpp:1957-1966` | `FLOAT` token → `FloatLiteralNode` with `is_single_precision` |
| AST | `src/ast.h:552-559` | `FloatLiteralNode` (value, `is_single_precision`) |
| Codegen — float literal (now SSE) | `src/codegen.cpp:1509-1532` | `FloatLiteralNode::visit` loads into `%xmm0` via `movd`/`movq` |
| Codegen — float load | `src/codegen.cpp:1547-1615` | `IdentifierExprNode::visit`: `movss`/`movsd` for float/double, `movl`/`mov` for int |
| Codegen — float store | `src/codegen.cpp:459-552` | `VarDeclNode::visit` allocates 4/8-byte slots and stores via `movss`/`movsd` |
| Codegen — float assign | `src/codegen.cpp:892-989` | `AssignExprNode::visit` writes float value to a variable slot via `movss`/`movsd` |
| Codegen — deref width | `src/codegen.cpp:1906-1939` | `DEREF` case in `generate_unary`: 1/2/4/8-byte `mov` family |
| Codegen — type sizes | `src/codegen.cpp:2065-2091` | `get_type_size`: `float=4`, `double=8` |
| Codegen — sizeof | `src/codegen.cpp:1120-1146` | `SizeofExprNode::visit` returns 4/8 for float/double |
| Codegen — `&f` type | `src/codegen.cpp:2334-2338` | `infer_expr_type` ADDRESS_OF returns `"<inner>*"` (e.g. `"float*"`) |
| Codegen — load from capture | `src/codegen.cpp:1584-1599` | Reads 1/2/4/8 bytes from a captured variable via the `__ctx` pointer |
