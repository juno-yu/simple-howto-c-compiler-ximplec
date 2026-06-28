# Lesson 0041: Multi-Dimensional Arrays

## Status: ⚠️ Partial | Phase: Advanced Types | Effort: Medium (6-8h)

## Objective

Implement `int arr[2][3]` and multi-dimensional indexing `arr[i][j]`.

## Implementation Status

| Feature | Status | Notes |
|---------|--------|-------|
| 2D declaration `int a[2][3]` | ✅ | Flattened to `int[6]` total size |
| 2D indexing `a[i][j]` | ✅ | Parser flattens to `a[i*3+j]` |
| 3D declaration `int a[2][3][4]` | ✅ | Flattened to `int[24]` total size |
| 3D indexing `a[i][j][k]` | ⚠️ | Only 2D flattened; 3rd dim loads value instead of address |
| 4D+ declaration | ✅ | Total size computed correctly |
| 4D+ indexing `a[i][j][k][l]` | ⚠️ | Only 2D flattened; 3rd+ dims broken |
| Brace init `{{1,2,3},{4,5,6}}` | ❌ | Nested braces cause segfault |
| Unspecified first dim `[][3]` | ❌ | Parser error or segfault |
| Pointer-to-array `(*p)[3]` | ❌ | Parser error |
| sizeof inner array `sizeof(a[0])` | ⚠️ | Returns pointer size, not inner array size |
| 2D+ multi-dim pointer decay | ❌ | Inner arrays don't decay to pointers |

## Current Limits

### Max dimensions supported: **2 (fully functional)**

The parser and codegen handle up to 2 dimensions. For 3D+, the first two dimensions are flattened at parse time, but the third+ dimensions use nested indexing which loads the **value** instead of taking the **address** — causing segfaults or wrong results.

### Why 3D+ breaks

For `int a[2][3][4]; a[1][2][3] = 99;`:

```
Parser flattening:
  a[1]       →  a + 1*3*4  = a + 12   (correct: address of row 1,2)
  a[1][2]    →  a[1*3+2]   = a[5]     (flattened to single index)
  a[1][2][3] →  a[5][3]    = ?        (loads VALUE of a[5], not address)
```

The codegen for `a[5]` (IndexExprNode on IdentifierExprNode) loads the integer at offset `5*4 = 20` from the base. Then `[3]` tries to index into that loaded integer as if it were an address — segfault.

To fix: the codegen must detect when an IndexExprNode result is used as the base of another IndexExprNode, and keep the **address** in `%rax` instead of loading the value.

### Max flattened dimensions: **2**

```
int a[2][3]       →  6 ints, 24 bytes   ✅ fully working
int a[2][3][4]    →  24 ints, 96 bytes  ⚠️ declaration works, indexing broken
int a[2][3][4][5] → 120 ints, 480 bytes ⚠️ declaration works, indexing broken
```

### What works now (2D)

```c
int a[2][3];
a[0][0] = 1;  a[0][1] = 2;  a[0][2] = 3;
a[1][0] = 4;  a[1][1] = 5;  a[1][2] = 6;
return a[1][2];  // → 6
```

This works because:
1. `int a[2][3]` → total size = 2*3*4 = 24 bytes (stack)
2. `a[1][2]` → parser flattens to `a[1*3+2]` = `a[5]` (single IndexExprNode)
3. codegen: `base + 5*4 = base + 20` → loads value at that address

### What doesn't work

```c
int a[2][3][4];
a[1][2][3] = 99;  // segfault (3D indexing)
return a[1][2][3];

int a[2][3] = {{1,2,3},{4,5,6}};  // segfault (nested braces)
return a[1][2];

int a[][3] = {{1,2,3},{4,5,6}};  // parser error (unspecified first dim)
return a[1][2];
```

### sizeof inner array

`sizeof(a[0])` for `int a[2][3]` should return 12 (3 ints × 4 bytes). Currently it returns 8 (pointer size) because the expression `a[0]` is an IndexExprNode, and the codegen doesn't know it's an array access.

## Future Work

1. **3D+ indexing**: when codegen sees `IndexExprNode(IndexExprNode(...), idx)`, and the inner result is a multi-dim array, compute address (don't load value). Add `inner_dim` tracking in IndexExprNode codegen.

2. **Nested brace init**: `int a[2][3] = {{1,2,3},{4,5,6}}` — parse `{{1,2,3},{4,5,6}}` as an InitializerList of InitializerLists, then store each sub-list at the right row offset.

3. **Unspecified first dim**: `int a[][3]` — deduce first dimension from initializer count. Requires counting elements in each sub-list at parse time.

4. **Pointer-to-array**: `(*p)[3]` — parse the `(*p)[3]` declarator syntax. Currently parser doesn't handle this.

5. **sizeof inner array**: track whether an IndexExprNode is an array access, and emit the inner array size instead of pointer size.

6. **Array pointer decay**: for 2D+, `a[1]` should decay to `int*` (address of row 1), not `int` (value). The codegen currently loads the value at `a[1]`.

## Source Code References

| Component | File | Description |
|-----------|------|-------------|
| Multi-dim declaration | `src/parser.cpp` | `parse_var_decl`: collapses `arr[M][N]` to total size M*N |
| Multi-dim flatten | `src/parser.cpp` | `parse_postfix`: detects `[i][j]` and computes `i*N+j` |
| Inner dim tracking | `src/parser.h` | `multidim_inner_dim_` map |
| Index codegen | `src/codegen.cpp` | `visit(IndexExprNode)`: `base + index * elem_size` |
| Array info | `src/codegen.cpp` | `array_info_[name] = {elem_size, count}` |
