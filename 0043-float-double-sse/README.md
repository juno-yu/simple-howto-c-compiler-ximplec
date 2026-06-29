# Lesson 0043: Float and Double (Real SSE Implementation)

## Status: ✅ Complete | Phase: Float & Advanced

## Objective

Implement **real IEEE 754 float/double arithmetic** using x86-64 SSE
instructions. Floats and doubles are loaded into XMM registers, operated on
with `addss`/`addsd`/`mulss`/`mulsd`/etc., compared with `ucomiss`/`ucomisd`,
and converted to/from integers with `cvtsi2ss`/`cvttss2si`/etc.

The System V x86-64 ABI is followed: float/double arguments go in `%xmm0`–`%xmm7`,
float/double return values go in `%xmm0`.

## Background

The previous lesson
([`0043-float-double-bit-pattern`](../0043-float-double-bit-pattern/README.md))
stored floats as raw 32/64-bit integer bit patterns and could only do
storage, address-of, and pointer-cast. It did not implement float arithmetic.

This lesson replaces that with proper SSE.

## Design

### Type-aware Expression Evaluation

The compiler tracks expression types via `expr_type_stack_` and the static
`infer_expr_type` helper (see `src/codegen.cpp:2296-2400`). Each sub-expression
leaves a type tag indicating whether the result is in `%xmm0` (float/double)
or `%rax` (integer/pointer).

| Type Tag   | Result Location | Width    |
|------------|-----------------|----------|
| `int`      | `%rax`          | 8 bytes  |
| `char`     | `%rax`          | 1 byte   |
| `float`    | `%xmm0`         | 4 bytes  |
| `double`   | `%xmm0`         | 8 bytes  |
| `T*`       | `%rax`          | 8 bytes  |

### Float Literal Emission

Float literals are loaded into `%xmm0` via bit-pattern transfer from `%eax` or
`%rax`. The `is_single_precision` flag carried on the `FloatLiteralNode`
selects between the two paths.

```cpp
// src/codegen.cpp:1509-1532
void CodeGenerator::visit(FloatLiteralNode& node) {
    double d = node.value;
    bool is_single = node.is_single_precision;
    if (is_single) {
        float f = static_cast<float>(d);
        uint32_t float_bits = 0;
        std::memcpy(&float_bits, &f, sizeof(float_bits));
        int32_t signed_bits;
        std::memcpy(&signed_bits, &float_bits, sizeof(signed_bits));
        emit("mov $" + std::to_string(signed_bits) + ", %eax");
        emit("movd %eax, %xmm0");
        push_expr_type("float");
    } else {
        uint64_t double_bits = 0;
        std::memcpy(&double_bits, &d, sizeof(double_bits));
        emit("movabsq $" + std::to_string(double_bits) + ", %rax");
        emit("movq %rax, %xmm0");
        push_expr_type("double");
    }
}
```

For `float f = 3.14f;` the generated assembly is:

```asm
mov  $0x4048F5C3, %eax      ; load bit pattern as 32-bit int
movd %eax, %xmm0            ; transfer to SSE register (zero-extend)
movss %xmm0, -8(%rbp)       ; store as 4-byte float
```

For `double d = 3.14;` the path uses `movabsq` + `movq` to load the 64-bit
pattern into `%xmm0`.

### Float Loads and Stores

For a local variable, the load width is `movss` (float, 4 bytes) or `movsd`
(double, 8 bytes):

```cpp
// src/codegen.cpp:1566-1581 (IdentifierExprNode::visit, float branch)
if (is_float_type(var_type)) {
    if (is_double_type(var_type)) {
        emit("movsd " + std::to_string(offset) + "(%rbp), %xmm0");
    } else {
        emit("movss " + std::to_string(offset) + "(%rbp), %xmm0");
    }
}
```

The store is symmetric: `VarDeclNode::visit` and `AssignExprNode::visit` emit
`movss`/`movsd` from `%xmm0` back to the stack slot.

### Binary Operations

The binary expression visitor computes the static type of the result via
`infer_expr_type`. When **either** operand is float/double, the SSE path is
taken. The right operand is evaluated first and spilled to a 16-byte temp
slot on the stack, so the left operand's evaluation (which may itself be a
nested float expression that clobbers `%xmm0`/`%xmm1`) does not destroy it.

```cpp
// src/codegen.cpp:1634-1674 (generate_binary, float path)
if (result_is_float) {
    const char* movop = result_is_double ? "movsd" : "movss";
    const char* cvtop = result_is_double ? "cvtsi2sd" : "cvtsi2ss";
    const char* suffix = result_is_double ? "sd" : "ss";

    // Evaluate right operand: result in %xmm0 (float) or %rax (int).
    dispatch(node.right.get());
    std::string actual_rtype = pop_expr_type();
    if (!is_float_type(actual_rtype)) {
        emit(std::string(cvtop) + " %rax, %xmm0");
    }
    emit("sub $16, %rsp");
    emit(std::string(movop) + " %xmm0, (%rsp)");

    // Evaluate left operand.
    dispatch(node.left.get());
    std::string actual_ltype = pop_expr_type();
    if (!is_float_type(actual_ltype)) {
        emit(std::string(cvtop) + " %rax, %xmm0");
    }
    emit(std::string(movop) + " (%rsp), %xmm1");
    emit("add $16, %rsp");

    switch (node.op) {
        case OpKind::ADD: emit(std::string("add") + suffix + " %xmm1, %xmm0"); break;
        case OpKind::SUB: emit(std::string("sub") + suffix + " %xmm1, %xmm0"); break;
        case OpKind::MUL: emit(std::string("mul") + suffix + " %xmm1, %xmm0"); break;
        case OpKind::DIV: emit(std::string("div") + suffix + " %xmm1, %xmm0"); break;
        ...
    }
    push_expr_type(result_is_double ? "double" : "float");
    return;
}
```

For mixed `int + float`, the integer is converted to float first via
`cvtsi2ss` / `cvtsi2sd`.

For comparison operators, `ucomiss` / `ucomisd` set `%eflags`, then
`sete`/`setne`/`setb`/`setbe`/`seta`/`setae` read the result and
`movzbq` zero-extends it into `%rax` (C's `int` result):

```cpp
// src/codegen.cpp:1684-1696
emit(std::string("ucomi") + suffix + " %xmm1, %xmm0");
switch (node.op) {
    case OpKind::EQ: emit("sete %al"); break;
    case OpKind::NE: emit("setne %al"); break;
    case OpKind::LT: emit("setb %al"); break;
    case OpKind::LE: emit("setbe %al"); break;
    case OpKind::GT: emit("seta %al"); break;
    case OpKind::GE: emit("setae %al"); break;
    default: break;
}
emit("movzbq %al, %rax");
```

The bitwise operators on float (`&`, `|`, `^`, `<<`, `>>`) are rejected with
an error message and a `xorpd %xmm0, %xmm0` zero.

### Unary Operations

Negation (`-x`) flips the IEEE 754 sign bit via a `xorps`/`xorpd` with the
appropriate mask constant. This is faster than `subss`/`subsd` from zero and
handles `-0.0` and NaN correctly per IEEE 754.

```cpp
// src/codegen.cpp:1877-1895 (generate_unary, UMINUS, float branch)
case OpKind::UMINUS:
    if (operand_is_float) {
        std::string actual = pop_expr_type();
        if (is_double_type(actual)) {
            emit("movabsq $0x8000000000000000, %rax");
            emit("movq %rax, %xmm1");
            emit("xorpd %xmm1, %xmm0");
        } else {
            emit("mov $0x80000000, %eax");
            emit("movd %eax, %xmm1");
            emit("xorps %xmm1, %xmm0");
        }
        push_expr_type(actual);
    } else {
        emit("neg %rax");
    }
    break;
```

### Conversions

`CastExprNode::visit` chooses between four branches based on the source and
destination type categories:

```cpp
// src/codegen.cpp:1148-1199
void CodeGenerator::visit(CastExprNode& node) {
    std::string src = infer_expr_type(node.expr.get());
    std::string dst = node.target_type;
    bool src_float = is_float_type(src);
    bool dst_float = is_float_type(dst);

    if (src_float && !dst_float) {
        // Float/double → integer: cvttss2si / cvttsd2si (truncation)
        dispatch(node.expr.get());
        std::string actual_src = pop_expr_type();
        if (is_double_type(actual_src))
            emit("cvttsd2si %xmm0, %rax");
        else
            emit("cvttss2si %xmm0, %rax");
        push_expr_type(dst);
        return;
    }
    if (!src_float && dst_float) {
        // Integer → float/double: cvtsi2ss / cvtsi2sd
        dispatch(node.expr.get());
        std::string actual_src = pop_expr_type();
        if (is_double_type(dst))
            emit("cvtsi2sd %rax, %xmm0");
        else
            emit("cvtsi2ss %rax, %xmm0");
        push_expr_type(dst);
        return;
    }
    if (src_float && dst_float) {
        // Float ↔ double: cvtss2sd / cvtsd2ss
        dispatch(node.expr.get());
        std::string actual_src = pop_expr_type();
        if (is_double_type(actual_src) && !is_double_type(dst))
            emit("cvtsd2ss %xmm0, %xmm0");
        else if (!is_double_type(actual_src) && is_double_type(dst))
            emit("cvtss2sd %xmm0, %xmm0");
        push_expr_type(dst);
        return;
    }
    // Integer to integer (or pointer): no-op
    ...
}
```

The integer-to-float path also fires automatically inside the SSE binary
emitter, since `cvtsi2ss` is emitted there when the static type of one
operand is integer.

### Calling Convention (System V x86-64 ABI)

For a function call, `CallExprNode::visit` walks the argument list twice:
once to count int and float args, then right-to-left evaluating each
argument. Float args are moved to the appropriate `%xmmN` register (or
spilled to the stack if there are more than 8); integer args are pushed
onto the stack and popped in order into `%rdi`, `%rsi`, ..., `%r9`.

```cpp
// src/codegen.cpp:1299-1333 (CallExprNode::visit, ABI dispatch)
int int_total = 0;
int float_total = 0;
for (int i = 0; i < num_args; i++) {
    std::string at = infer_expr_type(node.arguments[i].get());
    if (is_float_type(at)) float_total++;
    else int_total++;
}
int int_count = int_total;
int float_count = float_total;

for (int i = num_args - 1; i >= 0; i--) {
    dispatch(node.arguments[i].get());
    std::string actual = pop_expr_type();
    if (is_float_type(actual)) {
        int idx = --float_count;
        if (idx < 8) {
            std::string xreg = "%xmm" + std::to_string(idx);
            std::string mop = is_double_type(actual) ? "movsd" : "movss";
            if (xreg != "%xmm0") {
                emit(mop + " %xmm0, " + xreg);
            }
        } else {
            emit("sub $16, %rsp");
            emit("movsd %xmm0, (%rsp)");
        }
    } else {
        int idx = --int_count;
        if (idx < 6) {
            emit("push %rax");
        } else {
            emit("push %rax");
        }
    }
}
```

For the callee, `visit(FunctionDeclNode)` moves float params from
`%xmm0..%xmm7` into their stack slots, and integer params from
`%rdi..%r9`:

```cpp
// src/codegen.cpp:408-429 (visit(FunctionDeclNode), param handling)
static const char* param_regs[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
...
for (int i = 0; i < num_params; i++) {
    auto* pn = static_cast<ParamNode*>(node.params[i].get());
    std::string ptype = pn->type_name;
    bool is_fp = is_float_type(ptype);
    bool is_dbl = is_double_type(ptype);
    int psize = get_type_size(ptype);
    stack_offset_ += psize > 4 ? 8 : 8;
    int offset = -stack_offset_;
    local_variables_[pn->name] = offset;
    variable_types_[pn->name] = ptype;
    if (is_fp) {
        std::string xreg = "%xmm" + std::to_string(xmm_idx++);
        std::string mop = is_dbl ? "movsd" : "movss";
        emit(mop + " " + xreg + ", " + std::to_string(offset) + "(%rsp)");
    } else {
        if (int_idx < 6) {
            emit("mov " + std::string(param_regs[int_idx++]) + ", " + std::to_string(offset) + "(%rsp)");
        } else {
            int_idx++;
        }
    }
}
```

> **Note:** The snapshot above shows `(%rsp)` because the function body has
> not yet been allocated. After `sub $N, %rsp` and the `mov %rsp, %rbp`
> prologue, subsequent loads use `(%rbp)`. The example assembly in
> the "Example" section below shows the actual emitted output.

### Return Value Location

For `return expr;` in a function with a float/double return type, the
result is left in `%xmm0` rather than `%rax`. The function epilogue
preserves the XMM return register by simply not clobbering `%xmm0`:

```cpp
// src/codegen.cpp:712-745 (visit(ReturnStmtNode))
void CodeGenerator::visit(ReturnStmtNode& node) {
    bool ret_is_float = is_float_type(current_function_return_type_);
    if (node.value) {
        dispatch(node.value.get());
        if (ret_is_float) {
            std::string vt = infer_expr_type(node.value.get());
            if (!is_float_type(vt)) {
                if (is_double_type(current_function_return_type_))
                    emit("cvtsi2sd %rax, %xmm0");
                else
                    emit("cvtsi2ss %rax, %xmm0");
            }
        }
    } else {
        if (ret_is_float) {
            if (is_double_type(current_function_return_type_))
                emit("xorpd %xmm0, %xmm0");
            else
                emit("xorps %xmm0, %xmm0");
        } else {
            emit("mov $0, %rax");
        }
    }
    emit("mov %rbp, %rsp");
    emit("pop %rbp");
    emit("ret");
    returned_ = true;
}
```

When the function falls off the end without an explicit `return`, the
function epilogue zeros the appropriate register:

```cpp
// src/codegen.cpp:435-446 (visit(FunctionDeclNode), missing return)
if (!returned_) {
    if (is_float_type(current_function_return_type_)) {
        if (is_double_type(current_function_return_type_))
            emit("xorpd %xmm0, %xmm0");
        else
            emit("xorps %xmm0, %xmm0");
    } else {
        emit("mov $0, %rax");
    }
    emit_function_epilogue();
}
```

## Example

The example covers float add, double mul, int→float promotion, float
comparison, float→int cast, int→float cast, float sub, and float div.
Each `return N;` early-exits with a distinct non-zero value on failure
so a passing run returns 0:

```c
int main() {
    // Test 1: float add produces correct bits
    float a = 1.0f, b = 2.0f;
    float c = a + b;
    int *p = (int*)&c;
    if (*p != 0x40400000) return 1;

    // Test 2: double mul
    double d = 2.0 * 3.0;
    long *lp = (long*)&d;
    if (*lp != 0x4018000000000000L) return 2;

    // Test 3: int + float promotion
    float e = 1.0f + 2;
    int *ep = (int*)&e;
    if (*ep != 0x40400000) return 3;

    // Test 4: float comparison
    if (!(1.0f < 2.0f)) return 4;

    // Test 5: float to int cast
    if ((int)3.7f != 3) return 5;

    // Test 6: int to float cast
    float f = (float)42;
    int *fp = (int*)&f;
    if (*fp != 0x42280000) return 6;

    // Test 7: float subtraction
    float g = 5.0f - 2.0f;
    int *gp = (int*)&g;
    if (*gp != 0x40400000) return 7;

    // Test 8: float division
    float h = 6.0f / 2.0f;
    int *hp = (int*)&h;
    if (*hp != 0x40400000) return 8;

    return 0;
}
```

Verify it end-to-end:

```bash
./build/simplecc -S 0043-float-double-sse/src/example.c -o /tmp/example.s
gcc -o /tmp/example /tmp/example.s
/tmp/example ; echo "exit: $?"       # → exit: 0
```

The key fragment of the generated assembly for `float c = a + b;` is:

```asm
mov  $1065353216, %eax      ; 1.0f bit pattern
movd %eax, %xmm0
movss %xmm0, -8(%rbp)       ; store a
mov  $1073741824, %eax      ; 2.0f bit pattern
movd %eax, %xmm0
movss %xmm0, -16(%rbp)      ; store b
movss -16(%rbp), %xmm0      ; load b (right operand)
sub  $16, %rsp
movss %xmm0, (%rsp)         ; spill b to temp slot
movss -8(%rbp), %xmm0       ; load a (left operand)
movss (%rsp), %xmm1         ; restore b into %xmm1
add  $16, %rsp
addss %xmm1, %xmm0          ; a + b
movss %xmm0, -24(%rbp)      ; store c
```

For `double d = 2.0 * 3.0;` the path is the same with `movsd`/`mulsd`.

## What Doesn't Work (Future)

- `long double` (80-bit) — needs x87 FPU or 16-byte SSE
- `_Complex float` / `_Complex double`
- `<math.h>` library functions not inlined (`sin`/`cos`/`sqrt`)
- Variadic functions with float args (always goes through `%al` varargs count)
- FMA (fused multiply-add) — would need `vfmadd231ss` etc.
- `__attribute__((vector_size(N)))` SIMD types

## Source Code References

| Component | File | Description |
|-----------|------|-------------|
| Float literal | `src/codegen.cpp:1509-1532` | `FloatLiteralNode::visit` — `movd`/`movq` into `%xmm0` |
| Binary ops | `src/codegen.cpp:1625-1869` | `generate_binary` — float path uses `addss`/`subss`/`mulss`/`divss` + `ucomiss`+`setCC` |
| Unary ops | `src/codegen.cpp:1871-2062` | `generate_unary` — `UMINUS` flips sign bit via `xorps`/`xorpd` |
| Cast / conversion | `src/codegen.cpp:1148-1199` | `CastExprNode::visit` — `cvttss2si`/`cvtsi2ss`/`cvtss2sd`/`cvtsd2ss` |
| Function call ABI | `src/codegen.cpp:1287-1333` | `CallExprNode::visit` — split int vs xmm args, place per ABI |
| Function param ABI | `src/codegen.cpp:408-429` | `FunctionDeclNode::visit` — read params from `%rdi..%r9` and `%xmm0..%xmm7` |
| Return value | `src/codegen.cpp:712-745` | `ReturnStmtNode::visit` — leave float in `%xmm0`, integer in `%rax` |
| Fall-off return | `src/codegen.cpp:435-446` | `FunctionDeclNode::visit` — zero `%xmm0`/`%rax` if no explicit `return` |
| Identifier load | `src/codegen.cpp:1566-1581` | `IdentifierExprNode::visit` — `movss`/`movsd`/`movl`/`mov` based on type |
| Type sizes | `src/codegen.cpp:2065-2091` | `get_type_size`: `float=4`, `double=8` |
| Type inference | `src/codegen.cpp:2296-2400` | `infer_expr_type` walks the AST statically (no codegen) |
| Type helpers | `src/codegen.h:175-181` | `is_float_type`, `is_double_type` |
| AST node | `src/ast.h:552-559` | `FloatLiteralNode` (value, `is_single_precision`) |
| Lexer | `src/lexer.cpp:240-325` | Float literal lexing, `f` suffix, hex float |
| Parser | `src/parser.cpp:1957-1966` | `FLOAT` token → `FloatLiteralNode` with `is_single_precision` |
