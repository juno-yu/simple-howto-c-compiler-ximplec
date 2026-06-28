# Lesson 0038: Designated Initializers (C99)

## Status: ⚠️ Partial | Phase: Advanced Types | Effort: Medium (6-8h)

## Objective

Implement `.field = value` and `[index] = value` in initializer lists for structs and arrays.

## Implementation Status

| Feature | Status |
|---------|--------|
| Parse `.field = value` in struct initializers | ✅ |
| Parse `[index] = value` in array initializers | ✅ |
| Fill unspecified members with zeros | ⚠️ |
| Handle nested designators | ✅ (parsed) |
| Emit initializer values into storage | ❌ |

## Limitation

The lexer and parser accept designated initializer syntax and produce a valid AST. However, the codegen for initializer values is a no-op: the storage is allocated but **values are not emitted**. The default zero-initialization applies, so all struct/array members are zero after declaration.

A program like:

```c
struct Point p = {.x = 10, .y = 20};
```

will have `p.x == 0` and `p.y == 0` at runtime, not 10 and 20.

## Implementation Checklist

- [x] Parse `.field = value` in struct initializers
- [x] Parse `[index] = value` in array initializers
- [ ] Fill unspecified members with zeros
- [x] Handle nested designators (parsed)
- [ ] Error on duplicate designators
- [x] Parse braced init `{...}` 
- [ ] Emit `mov` instructions for each initializer value

## Source Code References

| Component | File | Description |
|-----------|------|-------------|
| Parser | `src/parser.cpp` (`parse_var_decl`) | Parses `{...}` and `.x = ...` designators |
| AST | `src/ast.h` | Initializer nodes |
| Codegen | `src/codegen.cpp` | Skips values, emits zero-initialized space |
