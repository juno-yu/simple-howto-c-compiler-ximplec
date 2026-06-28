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

## Limitation

The codegen emits all code from the AST without analyzing reachability. The current output for:

```c
int main() {
  int x = 1;
  if (0) {
    x = 999;     // unreachable but still emitted
  }
  return x;
}
```

includes the `mov $999, ...` instruction even though the branch is never taken.

## Future Work

1. Add a pass that tracks which statements are reachable.
2. After `return`, `break`, `continue`, `goto`, mark subsequent statements as dead.
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

Functionally correct (returns 1) but the assembly contains the dead `x = 999`.
