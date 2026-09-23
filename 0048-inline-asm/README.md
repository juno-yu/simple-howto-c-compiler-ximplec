# Lesson 0048: Inline Assembly

## Status: ✅ Complete (basic form) | Phase: System Integration | Effort: Medium

## Objective

Support GCC's `asm("...")` (and `__asm__("...")`) statement form. The string
literal is emitted verbatim into the output assembly so the user can drop
arbitrary x86-64 instructions into a function body.

## Syntax

```c
// Basic form — works
asm("mov $42, %rax");

// Volatile form — BROKEN: `volatile` lexes as KW_VOLATILE but the parser
// only checks for the identifier "volatile", so this errors out.
asm volatile("nop");

// Extended form — BROKEN: parse_asm_operands() leaves the operand's closing
// ')', so parsing fails before the AST node is built.
asm("mov %1, %0" : "=r"(result) : "r"(input) : "eax");
```

## AST Node

```cpp
// src/ast.h:532
struct AsmStmtNode : ASTNode {
    std::string assembly;
    std::string output;  // output operands
    std::string input;   // input operands
    std::string clobber; // clobbered registers

    AsmStmtNode(const std::string& asm_str, int l, int c)
        : ASTNode(NodeType::ASM_STMT, l, c), assembly(asm_str) {}
    void accept(ASTVisitor& visitor);
};
```

## Parsing

The parser treats `asm` as a statement-level keyword and consumes the
mandatory string literal. The intended `volatile` and extended-operand
handling is present in the code below but currently non-functional (see
Limitations):

```cpp
// src/parser.cpp:1025
// GCC inline assembly: asm("...") or asm volatile("...")
if (check(TokenType::IDENTIFIER) && peek().value == "asm") {
    advance(); // consume 'asm'
    if (check(TokenType::IDENTIFIER) && peek().value == "volatile") {
        advance(); // consume 'volatile' (optional)
    }
    if (!match(TokenType::LPAREN)) { /* error */ }
    if (!check(TokenType::STRING_LITERAL)) { /* error */ }
    std::string asm_str = peek().value;
    advance(); // consume string
    std::string output_ops, input_ops, clobber_regs;
    if (match(TokenType::COLON)) {
        if (!check(TokenType::COLON) && !check(TokenType::RPAREN)) {
            output_ops = parse_asm_operands();
        }
        if (match(TokenType::COLON)) {
            if (!check(TokenType::COLON) && !check(TokenType::RPAREN)) {
                input_ops = parse_asm_operands();
            }
            if (match(TokenType::COLON)) {
                if (!check(TokenType::RPAREN)) {
                    clobber_regs = parse_asm_clobbers();
                }
            }
        }
    }
    expect(TokenType::RPAREN);
    expect(TokenType::SEMICOLON);
    auto node = std::make_unique<AsmStmtNode>(asm_str, ...);
    node->output  = output_ops;
    node->input   = input_ops;
    node->clobber = clobber_regs;
    return std::move(node);
}
```

## Code Generation

The codegen emits the body string verbatim and a comment that shows the
original asm for readability:

```cpp
// src/codegen.cpp:1492
void CodeGenerator::visit(AsmStmtNode& node) {
    emit("# asm: " + node.assembly);
    output_ << "    " << node.assembly << "\n";
}
```

## Limitations

- **`asm volatile` does not parse.** The check is for an `IDENTIFIER` named
  `volatile`, but the lexer produces `KW_VOLATILE`, so `asm volatile("nop")`
  fails with `Expected ( after asm`.
- **Extended-asm lists do not parse.** `parse_asm_operands()` stops before
  consuming the operand's closing `)`, so `asm("..." : "=r"(x))` fails with
  `Expected ; but found )`. The `output`/`input`/`clobber` fields on
  `AsmStmtNode` are therefore never populated in practice.
- **No operand binding.** Even if the extended form parsed, constraint
  strings, expressions, and clobber lists would not be used: the output
  contains only the body string. To use `"=r"(x)` or clobbers the user must
  write the assembly by hand using actual register names.
- **No clobber tracking.** Caller-saved registers that the asm touches are
  not marked. The user's `asm` text is responsible for preserving `%rbp`,
  `%rsp`, `%rbx`, and any callee-saved register it uses.
- **No `%0`/`%1` substitution.** GCC's `asm` template uses positional
  operands; simplecc does not perform that rewrite — the user must write
  literal register names.
- **`asm goto` is not supported.**

## Example

```c
int main() {
    int result;
    asm("mov $42, %eax");
    return 42;
}
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| `AsmStmtNode` | `src/ast.h:532-541` | Holds assembly text and parsed operand lists |
| Parser | `src/parser.cpp:1025-1067` | `parse_statement` handles `asm` and extended-asm lists |
| Operand parser | `src/parser.cpp:1383-1400` | `parse_asm_operands` skips constraint strings and `(expr)` |
| Clobber parser | `src/parser.cpp:1402-1413` | `parse_asm_clobbers` collects clobber strings |
| Codegen | `src/codegen.cpp:1492-1496` | Emits body string verbatim |
| Visitor hook | `src/codegen.cpp:244` | `dispatch` routes `ASM_STMT` to the visitor |
