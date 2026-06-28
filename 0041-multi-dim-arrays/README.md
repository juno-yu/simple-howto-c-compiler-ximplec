# Lesson 0041: Multi-Dimensional Arrays

## Status: 📋 Planned | Phase: Advanced Types | Effort: Medium (6-8h)

## Objective

Implement `int arr[3][4]` and multi-dimensional indexing.

## Multi-Dimensional Array Layout

```mermaid
flowchart TD
    A["int m[3][4]"] --> B[Row-major layout in memory]
    B --> C["Row 0: m[0][0..3] = 16 bytes"]
    B --> D["Row 1: m[1][0..3] = 16 bytes"]
    B --> E["Row 2: m[2][0..3] = 16 bytes"]
    C --> F["Address: base + (i*cols + j)*sizeof(T)"]
    D --> F
    E --> F

    G["m[2][1] access"] --> H["i=2, j=1, cols=4, sizeof(int)=4"]
    H --> I["offset = (2*4 + 1)*4 = 36"]
    I --> J["Load from base + 36"]
```

## Implementation Checklist

- [ ] Parse multi-dimensional array declarations
- [ ] Row-major offset calculation: `arr[i][j] = base + i * cols * sizeof + j * sizeof`
- [ ] Array-to-pointer decay for each dimension
- [ ] Test: `int m[2][3] = {{1,2,3},{4,5,6}}; return m[1][1];` → 5
