# Lesson 0092: Nested Struct Member Access

## Status: ✅ Complete (member access) | Phase: Advanced Types | Effort: Easy

## Objective

Support `a.inner.x` for accessing members of nested (including anonymous) structs.

## Status

| Feature | Status |
|---------|--------|
| `a.inner.x` member access (compile) | ✅ Parsed; codegen emits correct offset arithmetic |
| Nested struct access at runtime | ✅ Fixed in `a0f740f` (recursive `compute_member_address`) |
| Nested struct initialization | ⚠️ Not supported (braced initializers produce zero-init storage) |

## Example

```c
struct A { struct { int x; } inner; };
int main() {
    struct A a;
    a.inner.x = 5;
    return a.inner.x;  // returns 5
}
```

The parser recognizes the anonymous `struct { ... }` field type and builds a nested `StructDeclNode`. At codegen time `compute_member_address()` recurses through nested `MemberExprNode`s, while `infer_member_expr_type()` and `get_field_type()` resolve the innermost field's type so the correct offset is added. Nested *initialization* (`struct A a = {{5}};` or `{.inner = {.x = 5}}`) still yields zero because braced initializer values are not yet emitted (see root README → Known Limitations → Initializers).

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Nested struct decl | `src/parser.cpp:415-425` | Handles anonymous `struct { ... }` fields |
| Member address (recursive) | `src/codegen.cpp:558-608` | `compute_member_address()` |
| Member type inference | `src/codegen.cpp:610-634` | `infer_member_expr_type()` |
| Field type lookup | `src/codegen.cpp:2223-2229` | `get_field_type()` |
| Member access codegen | `src/codegen.cpp:1541-1546` | `visit(MemberExprNode)` → `compute_member_address` |
