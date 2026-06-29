# Lesson 0097: Designated Initializers for Arrays

## Status: ⚠️ Partial | Phase: Advanced Types | Effort: Easy

## Objective

Support `int a[5] = {[2] = 10, [4] = 20};` for array designated initializers.

## Status

| Feature | Status |
|---------|--------|
| Struct designated init `.x = 1` | ✅ |
| Array designated init `[2] = 10` | ❌ Segfaults |

## Limitation

Array designated initializers (`[idx] = val`) segfault because the brace parser doesn't handle this syntax.

## Example (not yet working)

```c
int main() {
    int a[5] = {[2] = 10, [4] = 20};
    return a[2];  // expected 10, segfaults
}
```
