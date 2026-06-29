# Lesson 0019: String Literal Code Generation

## Status: ✅ Complete | Phase: String & Memory | Effort: Easy (4-6h)

## Objective

Generate `.rodata` section for string constants.

## String Literal Codegen Flow

```mermaid
graph TD
    A["\"hello\" in source"] --> B["String literal table"]
    B --> C[".rodata section with label"]
    C --> D["lea .Lstr_N(%rip), %rax"]
    D --> E["Load address into register"]
```

## Implementation Checklist

- [x] Add string literal table to codegen
      (`std::vector<StringLiteral> string_literals_`).
- [x] Generate `.rodata` section with labeled strings after `.text`.
- [x] Emit `lea label(%rip), %rax` to load string addresses.
- [x] Handle string escape sequences in the lexer (`\n`, `\t`, `\\`,
      `\"`, `\0`).
- [x] Test: `return "hello"[0];` → 104 ('h').
- [x] Test: `char *s = "hi"; return s[1];` → 105 ('i').

## Core Implementation Snippet — Literal Collection

Each `StringLiteralNode` is appended to the codegen's `string_literals_`
vector, then the address of the (not-yet-emitted) label is loaded into
`%rax`. The vector is later drained into `.rodata` at the end of
`generate()`.

```cpp
// src/codegen.cpp:1534
void CodeGenerator::visit(StringLiteralNode& node) {
    // Add string to literal table and emit address
    std::string label = ".Lstr_" + std::to_string(string_literals_.size());
    string_literals_.push_back({label, node.value});

    // Load address of string into %rax
    emit("lea " + label + "(%rip), %rax");
}
```

The `.rodata` section is emitted at the end of `generate()` so all
strings (even forward references) are collected first:

```cpp
// src/codegen.cpp:78
if (!string_literals_.empty()) {
    emit("");
    emit(".section .rodata");
    for (const auto& str : string_literals_) {
        emit_label(str.label);
        output_ << "    .asciz \"" << str.value << "\"\n";
    }
}
```

## Core Implementation Snippet — Lexer Escape Handling

```cpp
// src/lexer.cpp:327
Token Lexer::read_string() {
    int start_line = line_;
    int start_column = column_;
    advance();  // skip opening quote

    std::string str;
    while (!is_at_end() && peek() != '"') {
        if (peek() == '\\') {
            advance();
            if (is_at_end()) break;
            char c = advance();
            switch (c) {
                case 'n': str += '\n'; break;
                case 't': str += '\t'; break;
                case '\\': str += '\\'; break;
                case '"': str += '"'; break;
                case '0': str += '\0'; break;
                default:  str += c; break;
            }
        } else if (peek() == '\n') {
            error_message_ = "Unterminated string literal";
            return error_token("Unterminated string literal");
        } else {
            str += advance();
        }
    }
    if (is_at_end()) {
        error_message_ = "Unterminated string literal";
        return error_token("Unterminated string literal");
    }
    advance();  // skip closing quote
    return Token(TokenType::STRING_LITERAL, str, start_line, start_column);
}
```

## Example

```c
char *s = "hi";
return s[1];
```

Generates:

```asm
    .text
    .globl main
main:
    push %rbp
    mov %rsp, %rbp
    sub $128, %rsp
    lea .Lstr_0(%rip), %rax      ; "hi"
    mov %rax, -8(%rbp)
    mov -8(%rbp), %rax
    push %rax
    mov $1, %rax
    pop %rcx
    add %rcx, %rax               ; address of s[1]
    movzbl (%rax), %eax          ; load 'i' = 105
    mov %rbp, %rsp
    pop %rbp
    ret

    .section .rodata
.Lstr_0:
    .asciz "hi"
```

`./a.out; echo $?` → `105`.

## Implementation Details

### Source Code References

| Component | File | Lines | Description |
|-----------|------|-------|-------------|
| `STRING_LITERAL` token | src/token.h | 13 | Token type |
| `read_string()` | src/lexer.cpp | 327-365 | Scans string literal with escape handling |
| `StringLiteral` struct | src/codegen.h | 111-114 | `label`, `value` |
| `string_literals_` vector | src/codegen.h | 115 | Per-codegen-run string table |
| `generate()` clears table | src/codegen.cpp | 17 | `string_literals_.clear();` |
| `.rodata` section emission | src/codegen.cpp | 78-85 | Iterates `string_literals_`, emits `.asciz` for each |
| `visit(StringLiteralNode&)` | src/codegen.cpp | 1534-1541 | Allocates `.Lstr_N` label, pushes, emits `lea …(%rip), %rax` |
| `infer_expr_type()` for strings | src/codegen.cpp | 2305 | `STRING_LITERAL` → `"char*"` |
