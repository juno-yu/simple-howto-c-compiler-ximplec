# Lesson 0066: Constant Folding

## Status: ⚠️ Partial | Phase: Optimization | Effort: Easy

## Objective

Evaluate constant expressions at compile time to reduce runtime work.
For example, `2 + 3` should emit `mov $5, %rax` directly instead of
`mov $2, %rax; mov $3, %rcx; add %rcx, %rax`.

## Implementation Status

| Feature | Status |
|---------|--------|
| Fold `2 + 3` → `5` at compile time | ❌ Not implemented |
| Fold `x * 1` → `x` | ❌ Not implemented |
| Fold `x + 0` → `x` | ❌ Not implemented |
| Fold constant branches in `if (0) { ... }` | ❌ Not implemented |

## What Is There

The compiler has the **infrastructure** to do constant folding — AST
nodes carry integer values (e.g. `IntegerLiteralNode::value`) and the
codegen's `visit(BinaryExprNode)` is a single visitor that could be
short-circuited if both operands are known constants — but no
folding pass is currently implemented. Every `+`, `-`, `*`, `/`, `%`,
`==`, `<`, `>`, etc. on integer literals lowers to a real runtime
`add` / `sub` / `imul` / `idiv` / `cmp` instruction.

## Limitation

The current codegen does **not** perform constant folding. All
arithmetic is computed at runtime, even when both operands are known
at compile time. To add this optimization, an AST-level pass would
need to:

1. Walk each `BinaryExprNode` whose operands are both
   `IntegerLiteralNode`.
2. Replace the binary expression with a single `IntegerLiteralNode`
   containing the precomputed result.
3. Recursively fold nested expressions.

## Example

```c
int main() {
  int x = 2 + 3;    // currently emits: add $3, $2 at runtime
                    // should emit:    mov $5, %rax
  int y = x * 4;    // currently emits: imul at runtime
                    // should emit:    mov $20, %rax (if x is known)
  return x + y;     // returns 25 at runtime
}
```

The current output is functionally correct (still returns 25) but
does extra work at runtime.

## Future Work

Implement an AST visitor pass `ConstFolder` that runs between parsing
and codegen. It should handle:

- Arithmetic on integer constants (`+`, `-`, `*`, `/`, `%`)
- Comparison on integer constants (`==`, `!=`, `<`, `>`, `<=`, `>=`)
- Logical on boolean constants (`&&`, `||`, `!`)
- Bitwise on integer constants (`&`, `|`, `^`, `<<`, `>>`)
- Conditional (`?:`) when condition is constant
