# Lesson 0039: Compound Literals (C99)

## Status: ⚠️ Partial | Phase: Advanced Types | Effort: Medium (4-6h)

## Objective

Support C99 compound literal syntax: `(type){initializer-list}`. A compound literal creates an unnamed object with the given type and initial value, similar to a string literal but for any type.

## Syntax

```c
int *p = (int[]){1, 2, 3};
struct Point p = (struct Point){.x = 10, .y = 20};
```

## Implementation Status

| Feature | Status |
|---------|--------|
| Parse `(type){...}` syntax | ✅ |
| Type-checked against expected type | ✅ |
| Allocate storage for the object | ⚠️ |
| Emit initializer values | ❌ (zero-init) |
| Use in expression context | ✅ (parsed) |

## Limitation

The parser accepts the compound literal syntax. The codegen creates a placeholder expression that evaluates to 0 — the actual storage allocation and initializer values are not emitted. A program that uses a compound literal will see the storage allocated as zeros.

```c
int *p = (int[]){1, 2, 3};
return p[1];  // returns 0, not 2
```

## Source Code References

| Component | File | Description |
|-----------|------|-------------|
| Parser | `src/parser.cpp` (`parse_primary`) | Detects `(type){...}` |
| AST | `src/ast.h` | `CompoundLiteralNode` |
| Codegen | `src/codegen.cpp` | Emits 0 as the value |
