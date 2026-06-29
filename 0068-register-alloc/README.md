# Lesson 0068: Register Allocation

## Status: ⚠️ Partial | Phase: Optimization | Effort: Hard

## Objective

Allocate local variables to physical CPU registers instead of stack
slots when possible, to reduce memory traffic and improve
performance. A proper allocator spills only when register pressure
exceeds the available registers, and tracks caller/callee-saved
register usage at function boundaries.

## Implementation Status

| Feature | Status |
|---------|--------|
| Reserve registers for variables | ❌ Not implemented |
| Spill to stack on register pressure | ❌ Not implemented |
| Caller/callee-saved register tracking | ❌ Not implemented |
| Live range analysis | ❌ Not implemented |
| Graph coloring allocator | ❌ Not implemented |

## What Is There

All local variables are stored on the stack and loaded into `%rax`
for use. The codegen allocates a stack slot per variable in
`visit(VarDeclNode)` (`src/codegen.cpp:459-545`) and emits a
`mov <offset>(%rbp), %rax` (or sized variant) for every read of a
local.

Each expression like `a + b + c + d + e` becomes a sequence of:

```asm
mov -8(%rbp), %rax    ; load a
mov -16(%rbp), %rcx
add %rcx, %rax
mov -24(%rbp), %rcx
add %rcx, %rax
mov -32(%rbp), %rcx
add %rcx, %rax
mov -40(%rbp), %rcx
add %rcx, %rax
```

A proper register allocator would keep `a`, `b`, `c`, `d`, `e` in
registers across the expression, avoiding the stack traffic.

## Current Strategy

- `%rax` is used as the primary working register.
- Function arguments come in `%rdi`, `%rsi`, `%rdx`, `%rcx`, `%r8`,
  `%r9` (System V ABI).
- Caller-saved (`%rax`, `%rcx`, `%rdx`, `%rsi`, `%rdi`, `%r8`,
  `%r9`, `%r10`, `%r11`) and callee-saved (`%rbx`, `%rbp`, `%r12`,
  `%r13`, `%r14`, `%r15`) registers are not tracked.

## Future Work

1. Liveness analysis: compute which variables are live at each
   program point.
2. Build an interference graph.
3. Apply graph coloring (Chaitin's algorithm) or linear scan.
4. Generate register save/restore code at function entry/exit.
5. Spill excess variables to stack slots.
