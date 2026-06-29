# Lesson 0093: Nested Brace Initialization for Multi-Dim Arrays

## Status: ✅ Complete | Phase: Advanced Types | Effort: Medium

## Objective

Support `int a[2][3] = {{1,2,3},{4,5,6}};` for initializing multi-dimensional arrays.

## Status

| Feature | Status |
|---------|--------|
| `int a[3] = {1,2,3}` | ✅ |
| `int a[2][3] = {{1,2,3},{4,5,6}}` | ✅ |
| `int a[][3] = {{1,2,3},{4,5,6}}` | ⚠️ First dimension must be specified |

## How It Works

For multi-dim arrays, the parser stores the full dimension vector in `multidim_dims_`. The brace-init parser is reached via `parse_var_decl` → `parse_brace_initializer`, and the resulting flat initializer is applied to the same `VarDeclNode` that already knows the total `array_size`. Combined with the N-dim index flatten from lesson 0087, both reads and writes use the standard row-major linear offset.

## Example

```c
int main() {
    int a[2][3] = {{1,2,3},{4,5,6}};
    return a[1][2];  // expected 6
}
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Multi-dim dims | `src/parser.cpp:640-670` | Records `{2,3,...}` for the variable |
| Brace initializer | `src/parser.cpp:673-678` | `var->initializer = parse_brace_initializer()` |
| N-dim flatten (read/write) | `src/parser.cpp:1831-1900` | Stride math in `parse_postfix` |
| VarDecl codegen | `src/codegen.cpp:468-486` | Allocates `array_size * elem_size` bytes |
