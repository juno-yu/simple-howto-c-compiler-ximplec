# Lesson 0092: Nested Struct Member Access

## Status: ⚠️ Partial | Phase: Advanced Types | Effort: Easy

## Objective

Support `a.inner.x` for accessing members of nested structs.

## Status

| Feature | Status |
|---------|--------|
| `a.inner.x` member access (compile) | ✅ Parsed, codegen emits correct offset arithmetic |
| Nested struct access at runtime | ⚠️ Segfaults (codegen does not allocate a stack frame for the struct aggregate) |
| Nested struct initialization | ⚠️ Not supported (brace-init parser does not descend into sub-structs) |

## Example

```c
struct A { struct { int x; } inner; };
int main() {
    struct A a;
    a.inner.x = 5;
    return a.inner.x;  // compiles, but segfaults at runtime
}
```

The parser recognizes the `struct { ... }` field type, builds a `StructDeclNode` with one field, and `MemberExprNode` codegen computes the inner offset. Runtime access on an uninitialized stack-allocated struct is the failure mode.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Nested struct decl | `src/parser.cpp:812-838` | Walks inner `{ ... }` as a normal struct decl |
| Member access codegen | `src/codegen.cpp:1427-1433` | `visit(MemberExprNode)` → `compute_member_address` |
