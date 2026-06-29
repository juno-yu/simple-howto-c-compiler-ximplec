# Lesson 0095: Nested Struct Member Access

## Status: ⚠️ Partial | Phase: Advanced Types | Effort: Easy

## Objective

Support `a.inner.x` for accessing members of nested structs.

## Status

| Feature | Status |
|---------|--------|
| `a.inner.x` member access | ✅ Works |
| Nested struct initialization | ⚠️ Segfaults |

## Example

```c
struct A { struct { int x; } inner; };
int main() {
    struct A a;
    a.inner.x = 5;
    return a.inner.x;  // returns 5
}
```
