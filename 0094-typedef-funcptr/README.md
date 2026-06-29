# Lesson 0094: Typedef for Function Pointers

## Status: ⚠️ Partial | Phase: Advanced Types | Effort: Easy

## Objective

Support `typedef int (*func_t)(int);` for function pointer types.

## Status

| Feature | Status |
|---------|--------|
| `typedef int (*func_t)(int)` | ⚠️ Parsed but type not registered |

## Limitation

The typedef is parsed but the function pointer type is not stored in typedef_names_, so subsequent use of `func_t` as a type fails.

## Example (expected)

```c
int main() {
    typedef int (*func_t)(int);
    func_t f = 0;
    return 0;
}
```
