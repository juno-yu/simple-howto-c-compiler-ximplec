# Lesson 0096: Nested Brace Initialization for Multi-Dim Arrays

## Status: ⚠️ Partial | Phase: Advanced Types | Effort: Medium

## Objective

Support `int a[2][3] = {{1,2,3},{4,5,6}};` for initializing multi-dimensional arrays.

## Status

| Feature | Status |
|---------|--------|
| `int a[3] = {1,2,3}` | ✅ |
| `int a[2][3] = {{1,2,3},{4,5,6}}` | ❌ Segfaults |
| `int a[][3] = {{1,2,3},{4,5,6}}` | ❌ Segfaults |

## Limitation

Nested braces cause a segfault because the brace parser doesn't handle nested `{...}` groups for sub-arrays.

## Example (not yet working)

```c
int main() {
    int a[2][3] = {{1,2,3},{4,5,6}};
    return a[1][2];  // expected 6, segfaults
}
```
