# Lesson 0100: N-Dimensional Arrays

## Status: ✅ Complete | Phase: Advanced Types | Effort: Medium

## Objective

Support arbitrary-dimensional arrays: `int a[2][3][4]`, `int b[2][3][4][5]`, etc.

## How It Works

### Declaration

`int a[2][3][4]` allocates 2×3×4×4 = 96 bytes on the stack. The full dimension vector {2, 3, 4} is stored in `multidim_dims_[name]`.

### Indexing

The parser detects `a[i][j][k]` on a known multi-dim identifier and flattens all indices at parse time using strides:

```
dims = {2, 3, 4}
strides = {3×4=12, 4, 1}

a[i][j][k]  →  i×12 + j×4 + k   (single flat IndexExprNode)
```

The codegen then does: `base + (i×12 + j×4 + k) × 4`.

### Example

```c
int a[2][3][4];
a[1][2][3] = 99;
return a[1][2][3];  // flat: 1×12 + 2×4 + 3 = 23, offset 23×4 = 92
```

## Tested Dimensions

| Dims | Declaration | Index Example | Result |
|------|-------------|---------------|--------|
| 1D | `int a[10]` | `a[9]` | ✅ |
| 2D | `int a[2][3]` | `a[1][2]` | ✅ |
| 3D | `int a[2][3][4]` | `a[1][2][3]` | ✅ |
| 4D | `int b[2][3][4][5]` | `b[1][2][3][4]` | ✅ |
| 5D | `int e[2][3][4][5][6]` | `e[1][2][3][4][5]` | ✅ |

## Current Limits

- **Brace initializer**: `int a[2][3] = {{1,2,3},{4,5,6}}` — not supported (nested braces)
- **Unspecified first dim**: `int a[][3] = ...` — not supported
- **Pointer-to-array**: `(*p)[3]` — not supported
- **sizeof inner array**: `sizeof(a[0])` returns pointer size, not inner array size
- **Array pointer decay**: `a[1]` doesn't decay to `int*` for 2D+

## Source Code References

| Component | File | Description |
|-----------|------|-------------|
| Multi-dim declaration | `src/parser.cpp` (`parse_var_decl`) | Stores dims in `multidim_dims_` |
| N-dim flatten | `src/parser.cpp` (`parse_postfix`) | Collects all indices, computes strides, flattens |
| Dimension tracking | `src/parser.h` | `multidim_dims_[name] = {dim0, dim1, ...}` |
| Index codegen | `src/codegen.cpp` (`visit(IndexExprNode)`) | `base + flat_index × elem_size` |
