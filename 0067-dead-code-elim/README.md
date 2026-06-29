# Lesson 0067: Dead Code Elimination

## Status: ⚠️ Partial | Phase: Optimization | Effort: Medium

## Objective

Remove code that has no effect on the program's output. This includes:

- Unreachable code after `return`, `break`, `continue`, `goto`.
- Unused variable declarations.
- Code inside `if (0) { ... }` (constant-false conditions).

## Implementation Status

| Feature | Status |
|---------|--------|
| Skip code after `return` | ❌ Not implemented (still emitted) |
| Skip code in `if (0)` | ❌ Not implemented |
| Remove unused variables | ❌ Not implemented |
| Remove unused functions | ❌ Not implemented |

## What Is There

The codegen is a straight-line visitor: every statement in a block
is visited in order (`src/codegen.cpp:706-710`), and each visit emits
the corresponding instructions. There is no reachability analysis,
no liveness analysis, and no "is the result of this expression used?"
check. The result is correct but unoptimized.

The infrastructure for DCE is in place — every statement in the
AST has a `void accept(ASTVisitor&)` method, so a tree-walking pass
that marks unreachable subtrees and replaces them with empty
statements would integrate cleanly. The current codegen would then
emit nothing for the marked statements.

## Limitation

The codegen emits all code from the AST without analyzing
reachability. The current output for:

```c
int main() {
  int x = 1;
  if (0) {
    x = 999;     // unreachable but still emitted
  }
  return x;
}
```

includes the `mov $999, ...` instruction even though the branch is
never taken.

## Future Work

1. Add a pass that tracks which statements are reachable.
2. After `return`, `break`, `continue`, `goto`, mark subsequent
   statements as dead.
3. After folding, treat `if (constant-false)` as dead.
4. Use a whole-program analysis to remove unused functions.
5. Use a local analysis to remove unused local variables.

## Example

```c
int main() {
  int x = 1;
  if (0) {
    x = 999;       // dead — should be removed
  }
  return x;        // returns 1
}
```

Functionally correct (returns 1) but the assembly contains the dead
`x = 999`.
