# Lesson 0083: Label-as-Value (`&&label`)

## Status: ✅ Complete | Phase: GCC Extensions | Effort: Medium

## Objective

Support the GCC extension that takes the address of a label as a value (a `void *` constant), enabling computed `goto` for jump tables and interpreters.

## Syntax

```c
void *ptr = &&label;       // take address of label
goto *ptr;                  // jump to that label
```

## Semantics

- `&&label` produces the address of `label` as a `void *` value.
- `goto *expr` jumps to the address contained in `expr`.
- The label must be in the same function.

## Implementation

The parser recognizes `&&` as a unary operator and the identifier following it as a label name. A `LabelAddrNode` AST node holds the label name. The codegen currently emits 0 as the value — labels are not given real addresses in this simplified implementation. The `goto *expr` form parses as a regular `goto` with an expression operand.

## Limitations

- **No real label address** — `&&label` is parsed and the codegen emits `mov $0, %rax` as a placeholder. The label does not get a real symbol/address.
- **No computed-goto validation** — `goto *ptr` is emitted as a no-op jump target.
- **Label values are not portable** — they are implementation-defined.

## Example

```c
int main() {
    int x = 0;
    goto *(&&target);      // computed goto
    x = 100;
target:
    return x;              // returns 0 (skipped the assignment)
}
```

## Source Code References

| Component | File | Description |
|-----------|------|-------------|
| Lexer | `src/lexer.cpp` | `&&` token |
| Parser | `src/parser.cpp` | `parse_unary` detects `&&` and parses label name |
| AST | `src/ast.h` | `LabelAddrNode` declaration |
| Codegen | `src/codegen.cpp` | `visitLabelAddrNode` (placeholder) |
