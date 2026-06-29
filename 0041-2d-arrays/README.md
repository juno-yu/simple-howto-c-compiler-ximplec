# Lesson 0041: 2D Arrays

## Status: ✅ Complete | Phase: Advanced Types | Effort: Medium

## Objective

Implement `int arr[2][3]` with 2D indexing `arr[i][j]`. The
parser flattens N dimensions into a single index at parse time,
so the codegen always sees a 1D `IndexExprNode`.

## How It Works

### Declaration

`int a[2][3]` allocates 2×3×4 = 24 bytes on the stack. The
total element count (6) is stored in `array_info_` as the
single `array_size` field. The full dimension list is also
saved in `multidim_dims_[name]` so the parser can compute
strides when it sees `a[i][j]`.

### Indexing

The parser detects `[i][j]` on an identifier and flattens to
`[i*3 + j]`. The codegen then does `base + (i*3 + j) * 4`.

```text
a[i][j]  →  parser rewrites to  a[i*3 + j]
```

The flattening is done in `parse_postfix()` after collecting
all bracketed indices. For N dimensions, a general formula
applies: for `dims = {D0, D1, ..., Dn-1}`, the strides are
`{D1*D2*...*Dn-1, D2*...*Dn-1, ..., 1}`, and the flat index
is `Σ indices[k] * strides[k]`.

### Example

```c
// src/example.c
int main() {
    int a[2][3];
    a[0][0] = 1;  a[0][1] = 2;  a[0][2] = 3;
    a[1][0] = 4;  a[1][1] = 5;  a[1][2] = 6;
    return a[1][2]; // → 6
}
```

## Implementation Details

The core trick: do the index arithmetic **at parse time** so
the codegen never has to know about 2D vs 1D.

### Parser — flatten `[i][j]`

In `parse_postfix()`, when more than one `[...]` is seen in
sequence on the same base expression, the parser computes
strides from the saved `multidim_dims_` and builds a single
`IndexExprNode` with a synthetic flat index
(`src/parser.cpp:1797-1938`):

```cpp
// src/parser.cpp:1831-1908 (abridged)
} else if (match(TokenType::LBRACKET)) {
    auto first_index = parse_expression();
    expect(TokenType::RBRACKET);

    // Collect all remaining [indices] for N-dim flattening
    std::vector<ASTPtr> indices;
    indices.push_back(std::move(first_index));
    while (check(TokenType::LBRACKET)) {
        advance();
        indices.push_back(parse_expression());
        expect(TokenType::RBRACKET);
    }

    // Try to flatten N dimensions
    std::string arr_name = get_array_name(primary.get());
    std::vector<int>* dims = arr_name.empty() ? nullptr : get_dims(arr_name);

    if (dims && indices.size() > 1) {
        // N-dim flatten: compute flat index from dims and indices
        int ndims = dims->size();
        std::vector<int> strides(ndims, 1);
        for (int i = ndims - 2; i >= 0; i--) {
            strides[i] = strides[i + 1] * (*dims)[i + 1];
        }
        // Build flat = indices[0]*strides[0] + indices[1]*strides[1] + ...
        ...
        auto result = std::make_unique<IndexExprNode>(line, col);
        result->array = std::move(primary);
        result->index = std::move(flat);
        primary = std::move(result);
    } else {
        // Single-dim: just nest
        ...
    }
}
```

The strides array is built right-to-left: `strides[N-1] = 1`,
then each `strides[i] = strides[i+1] * dims[i+1]`. The
expression built is a left-leaning sum of `index[k] * stride[k]`
terms.

### Parser — declaration collapse

`parse_var_decl()` collects all bracketed dimensions and
multiplies them into a single `var->array_size` for the
codegen (`src/parser.cpp:640-671`):

```cpp
// src/parser.cpp:640-671 (abridged)
if (match(TokenType::LBRACKET)) {
    std::vector<int> dims;
    if (check(TokenType::INTEGER)) {
        dims.push_back(std::stoi(peek().value));
        var->array_size = dims[0];
        advance();
    } else {
        dims.push_back(0);
    }
    expect(TokenType::RBRACKET);
    while (check(TokenType::LBRACKET)) {
        advance();
        int dim_size = 0;
        if (check(TokenType::INTEGER)) {
            dim_size = std::stoi(peek().value);
            dims.push_back(dim_size);
            advance();
        } else {
            dims.push_back(0);
        }
        expect(TokenType::RBRACKET);
        if (dim_size > 0) {
            var->array_size = (var->array_size > 0 ? var->array_size : 1) * dim_size;
        }
    }
    if (dims.size() > 1) {
        multidim_dims_[var->name] = dims;
    }
}
```

So `int a[2][3]` ends up with `array_size = 6` and
`multidim_dims_["a"] = {2, 3}`. The codegen sees a 6-element
flat array of 4-byte ints — 24 bytes on the stack.

## Example

For `a[1][2]`, the parser sees `dims = {2, 3}` and indices
`{1, 2}`. Strides are `{3, 1}`. The flat index is
`1*3 + 2*1 = 5`. The codegen sees `a[5]` and emits
`base + 5*4` — same as the 1D case.

```asm
    lea -24(%rbp), %rax       # &a[0]
    push %rax
    mov $1, %rax               # i = 1
    imul $3, %rax              # × stride[0] = 3
    push %rax
    mov $2, %rax               # j = 2
    imul $1, %rax              # × stride[1] = 1
    pop %rcx                   # 1*3 = 3
    add %rcx, %rax             # 3 + 2 = 5
    pop %rcx                   # base
    add %rcx, %rax             # base + 5
    movl (%rax), %eax          # load
```

## Source Code References

| Component | File | Lines | Description |
|-----------|------|-------|-------------|
| Multi-dim declaration | `src/parser.cpp` | `640-671` | Collapses `arr[M][N]` to total size M×N |
| Multi-dim flatten | `src/parser.cpp` | `1831-1908` | Rewrites `a[i][j]` to `a[i*N + j]` |
| `multidim_dims_` | `src/parser.h` | `~44` | `map<name, vector<int>>` |
| `multidim_inner_dim_` | `src/parser.h` | (older alias) | Single inner-dim fallback |
| Index codegen | `src/codegen.cpp` | `1367-1425` | `base + index * elem_size` |
| Array info | `src/codegen.h` | `146-150` | `elem_size`, `length` |

## Status

- **Parser / Codegen**: ✅ 2D arrays work, including
  out-of-order and non-constant indices (the flattening
  arithmetic is runtime).
- **N dimensions**: ✅ General N-dim arrays are supported —
  the flattening code computes arbitrary strides.
- **Note (size must be constant)**: All dimensions must be
  constant integer expressions at declaration time. A
  declaration like `int a[n][3]` (variable first dim) is
  parsed as `array_size = n*3` only if `n` is an integer
  literal.
