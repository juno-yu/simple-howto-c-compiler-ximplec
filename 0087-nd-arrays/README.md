# Lesson 0087: N-Dimensional Arrays

## Status: ✅ Complete | Phase: Advanced Types | Effort: Medium

## Objective

Support arbitrary-dimensional arrays: `int a[2][3][4]`, `int b[2][3][4][5]`, etc.

## How It Works

### Declaration

`int a[2][3][4]` allocates 2×3×4×4 = 96 bytes on the stack. The full dimension vector {2, 3, 4} is stored in `multidim_dims_[name]`.

```cpp
// src/parser.cpp:640-670
expect(TokenType::LBRACKET);
int dim_size = 0;
if (check(TokenType::INTEGER)) {
    dim_size = std::stoi(peek().value);
    dims.push_back(dim_size);
    advance();
} else {
    dims.push_back(0);
}
expect(TokenType::RBRACKET);
// ... additional [N] pairs ...
if (dims.size() > 1) {
    multidim_dims_[var->name] = dims;
}
```

### Indexing (N-dim flatten)

The parser collects all bracketed indices at the postfix level and flattens them using row-major strides at parse time:

```cpp
// src/parser.cpp:1844-1898 — inside parse_postfix
// For int a[M][N][P]: strides = {N*P, P, 1}
int ndims = dims->size();
std::vector<int> strides(ndims, 1);
for (int i = ndims - 2; i >= 0; i--) {
    strides[i] = strides[i + 1] * (*dims)[i + 1];
}
// Build flat index: sum of indices[i] * strides[i]
// Wrap in single IndexExprNode for lvalue
auto result = std::make_unique<IndexExprNode>(line, col);
result->array = std::move(primary);
result->index = std::move(flat);
```

```
dims   = {2, 3, 4}
strides = {3×4=12, 4, 1}

a[i][j][k]  →  i×12 + j×4 + k   (single flat IndexExprNode)
```

The codegen then computes: `base + (i×12 + j×4 + k) × 4`.

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

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Multi-dim declaration | `src/parser.cpp:640-670` | Stores dims in `multidim_dims_` |
| Multi-declarator dims | `src/parser.cpp:701-728` | Same for `int a, b[N][M];` lists |
| N-dim flatten | `src/parser.cpp:1831-1900` | Collects indices, computes strides, flattens |
| Dimension map | `src/parser.h:94` | `multidim_dims_[name] = {dim0, dim1, ...}` |
| Index codegen | `src/codegen.cpp:1367-1425` | `base + flat_index × elem_size` |
