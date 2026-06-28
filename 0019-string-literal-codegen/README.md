# Lesson 0019: String Literal Code Generation

## Status: ✅ Complete | Phase: String & Memory | Effort: Easy (4-6h)

## Objective

Generate `.rodata` section for string constants.

## String Literal Codegen Flow

```mermaid
graph TD
    A["\"hello\" in source"] --> B["String literal table"]
    B --> C[".rodata section with label"]
    C --> D["lea .LC0(%rip), %rax"]
    D --> E["Load address into register"]
```

## Implementation Checklist

- [ ] Add string literal table to codegen
- [ ] Generate `.rodata` section with labeled strings
- [ ] Emit `lea label(%rip), %rax` to load string addresses
- [ ] Handle string escape sequences
- [ ] Test: `return "hello"[0];` → 104 ('h')
- [ ] Test: `char *s = "hi"; return s[1];` → 105 ('i')

## Implementation Details

| File | Lines | Description |
|------|-------|-------------|
| `src/lexer.cpp` | 247–284 | `read_string()` scans string literals with escape sequence handling (`\n`, `\t`, `\\`, `\"`, `\0`) |
| `src/codegen.h` | 104–109 | `StringLiteral` struct (`label`, `value`) and `string_literals_` vector |
| `src/codegen.cpp` | 13 | `generate()` clears `string_literals_` at start |
| `src/codegen.cpp` | 45–53 | `.rodata` section emission — iterates `string_literals_`, emits `.asciz` directives |
| `src/codegen.cpp` | 929–935 | `visit(StringLiteralNode&)` — creates `.Lstr_N` label, pushes to table, emits `lea label(%rip), %rax` |
| `src/codegen.cpp` | 956–958 | `visit(IdentifierExprNode&)` — loads string variable addresses via `lea name(%rip), %rax` |

## Source Code References

- **Lexer string scanning**: `src/lexer.cpp:247-284` — `read_string()` handles escape sequences, returns `STRING_LITERAL` token
- **String literal table**: `src/codegen.h:104-109` — `StringLiteral` struct and `string_literals_` storage
- **Rodata section**: `src/codegen.cpp:45-53` — emits `.section .rodata` with `.asciz` for each string
- **Address loading**: `src/codegen.cpp:929-935` — `lea .Lstr_N(%rip), %rax` loads string address into register
- **Token type**: `src/token.h` — `STRING_LITERAL` token type

## Status

- **Lexer**: ✅ Full string literal scanning with escape sequences
- **Codegen**: ✅ String table, `.rodata` section emission, and `lea` address loading implemented
