# Lesson 0025: Array Types

## Status: 📋 Planned | Phase: Data Structures | Effort: Hard (8-12h)

## Objective

Implement fixed-size arrays with indexing.

## Array Layout and Indexing

```mermaid
flowchart TD
    A["int arr[3] = {10, 20, 30}"] --> B[Stack Allocation]
    B --> C["arr[0] at rbp-12 = 10"]
    B --> D["arr[1] at rbp-8 = 20"]
    B --> E["arr[2] at rbp-4 = 30"]

    F["arr[i] access"] --> G["Compute address: base + i * sizeof(T)"]
    G --> H["base = rbp - 12"]
    H --> I["offset = i * 4"]
    I --> J["Load from base + offset"]

    K["Multi-dimensional: int m[3][4]"] --> L["Row 0: m[0][0..3]"]
    K --> M["Row 1: m[1][0..3]"]
    K --> N["Row 2: m[2][0..3]"]
    L --> O["base + (i*4 + j) * 4"]
```

## Implementation Checklist

- [ ] Parse array declarations: `int arr[10]`
- [ ] Parse array initializers: `int arr[] = {1, 2, 3}`
- [ ] Stack allocation for arrays
- [ ] Index expression codegen: `base + i * sizeof(type)`
- [ ] Array-to-pointer decay
- [ ] Multi-dimensional arrays: `int m[3][4]`
- [ ] Test: `int a[3] = {10, 20, 30}; return a[1];` → 20
