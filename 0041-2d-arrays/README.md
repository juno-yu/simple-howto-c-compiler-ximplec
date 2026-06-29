# Lesson 0041: 2D Arrays

## Status: ✅ Complete | Phase: Advanced Types | Effort: Medium

## Objective

Implement `int arr[2][3]` with 2D indexing `arr[i][j]`. The parser flattens 2 dimensions into a single index at parse time.

## How It Works

### Declaration

`int a[2][3]` allocates 2×3×4 = 24 bytes on the stack. The total element count (6) is stored in `array_info_`.

### Indexing

The parser detects `[i][j]` on an identifier and flattens to `[i*3 + j]`:
```
a[i][j]  →  parser rewrites to  a[i*3 + j]
```
The codegen then does: `base + (i*3 + j) * 4`.

### Example

```c
int a[2][3];
a[1][2] = 6;  // parser: a[1*3 + 2] = a[5], offset 5*4 = 20
return a[1][2]; // → 6
```

## Source Code References

| Component | File | Description |
|-----------|------|-------------|
| Multi-dim declaration | `src/parser.cpp` (`parse_var_decl`) | Collapses `arr[M][N]` → total size M×N |
| Multi-dim flatten | `src/parser.cpp` (`parse_postfix`) | Detects `[i][j]` and computes `i*N+j` |
| Inner dim tracking | `src/parser.h` | `multidim_inner_dim_[name] = N` |
| Index codegen | `src/codegen.cpp` (`visit(IndexExprNode)`) | `base + index × elem_size` |
