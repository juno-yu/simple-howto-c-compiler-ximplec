# Lesson 0042: Array-to-Pointer Decay

## Status: 📋 Planned | Phase: Advanced Types | Effort: Easy (2-3h)

## Objective

Implement automatic conversion from array to pointer.

## Array-to-Pointer Decay

```mermaid
flowchart TD
    A["int arr[3] = {1,2,3}"] --> B["arr in expression"]
    B --> C["Decays to: &arr[0]"]
    C --> D["int* pointer to first element"]

    E["Exceptions to decay"] --> F["sizeof(arr) → full array size"]
    E --> G["&arr → pointer to array type"]
    E --> H["String literal → char*"]

    I["Function parameter"] --> J["void f(int a[3])"]
    J --> K["Equivalent to: void f(int *a)"]
    K --> L["Parameter is pointer, not array"]
```

## Implementation Checklist

- [ ] Array name in expression → pointer to first element
- [ ] Exception: `sizeof(array)` returns full array size
- [ ] Exception: `&array` returns pointer to array
- [ ] Function parameters: arrays become pointers
- [ ] Test: `int a[3] = {1,2,3}; int *p = a; return *p;` → 1
