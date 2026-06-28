# Lesson 3011: auto Arrays (C23)

## Status: 📋 Planned | Standard: C23 | Effort: Easy

## Objective

Infer array size from initializer.

## Syntax

```c
auto arr[] = {1, 2, 3};       // int[3]
auto arr2[] = {1.0, 2.0};     // double[2]
```

## Implementation Checklist

- [ ] Parse `auto arr[] = {init-list}`
- [ ] Infer element type from initializer
- [ ] Infer size from number of elements
- [ ] Test: `auto a[] = {1,2,3}; return sizeof(a);` → 12
