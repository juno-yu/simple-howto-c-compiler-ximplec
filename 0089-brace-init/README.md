# Lesson 0089: Brace Initializers for Structs

## Status: ⚠️ Partial | Phase: Advanced Types | Effort: Easy

## Objective

Support `struct S s = {1, 2};` for initializing structs.

## Status

| Feature | Status |
|---------|--------|
| Simple brace init `struct S s = {1,2}` | ✅ |
| Positional init for arrays `int a[3] = {1,2,3}` | ✅ |
| Nested brace init `{{1,2,3},{4,5,6}}` | ❌ Segfaults |
| Designated init `.field = val` | ✅ For structs |
| Designated init `[idx] = val` | ✅ For arrays |

## Example

```c
struct S { int x; int y; };
int main() {
    struct S s = {1, 2};  // s.x=1, s.y=2
    return s.y;           // returns 2
}
```
