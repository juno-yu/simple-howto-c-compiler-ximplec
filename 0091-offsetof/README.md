# Lesson 0091: __builtin_offsetof

## Status: ⚠️ Partial | Phase: Advanced Types | Effort: Easy

## Objective

Support `__builtin_offsetof(type, member)` for computing struct field offsets.

## Status

| Feature | Status |
|---------|--------|
| `__builtin_offsetof(struct S, y)` | ✅ Computes correct offset |
| `__builtin_offsetof` in array init | ❌ Segfaults |

## Example

```c
struct S { int x; int y; };
int main() {
    return __builtin_offsetof(struct S, y);  // returns 4
}
```
