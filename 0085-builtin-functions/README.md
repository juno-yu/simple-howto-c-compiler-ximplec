# Lesson 0085: Built-in Functions (`__builtin_*`)

## Status: ✅ Complete | Phase: GCC Extensions | Effort: Easy

## Objective

Recognise and lower the most common GCC built-in functions.
Builtins are functions the compiler recognises and replaces with
optimized code or special lowering — they are not real functions
and do not require declarations.

## Supported Builtins

| Builtin | Status | Lowering |
|---------|--------|----------|
| `__builtin_expect(expr, expected)` | ✅ | Returns `expr` unchanged (hint to branch predictor) |
| `__builtin_offsetof(type, member)` | ✅ | Computes byte offset of `member` in `type` at compile time |
| `__builtin_popcount(x)` | ✅ | Emits `popcnt %eax, %eax` |
| `__builtin_va_list`, `__builtin_va_start`, etc. | ❌ | Not implemented; variadic args unsupported |

The builtins are recognised by name in the lexer as
`TokenType::IDENTIFIER` (no special token — they are ordinary
identifiers at the lexical level):

```cpp
// src/lexer.cpp:152-155
{"__builtin_offsetof", TokenType::IDENTIFIER},
{"__builtin_expect",   TokenType::IDENTIFIER},
{"__builtin_popcount", TokenType::IDENTIFIER},
```

The codegen's `visit(CallExprNode)` checks the callee name and
lowers each builtin:

```cpp
// src/codegen.cpp:1204
// Handle __builtin_expect: just evaluate first argument
if (node.function_name == "__builtin_expect") {
    if (!node.arguments.empty()) {
        dispatch(node.arguments[0].get());
    }
    return;
}

// Handle __builtin_offsetof(type, member): return offset
if (node.function_name == "__builtin_offsetof" && node.arguments.size() == 2) {
    if (auto* type_arg = dynamic_cast<IdentifierExprNode*>(node.arguments[0].get())) {
        if (auto* member_arg = dynamic_cast<IdentifierExprNode*>(node.arguments[1].get())) {
            std::string type_name = type_arg->name;
            std::string member_name = member_arg->name;
            // Strip struct/union prefix
            if (type_name.substr(0, 7) == "struct ") type_name = type_name.substr(7);
            else if (type_name.substr(0, 6) == "union ") type_name = type_name.substr(6);
            int offset = get_field_offset(type_name, member_name);
            emit("mov $" + std::to_string(offset >= 0 ? offset : 0) + ", %rax");
            return;
        }
    }
    emit("mov $0, %rax");
    return;
}

// Handle __builtin_popcount: inline popcount using popcnt instruction
if (node.function_name == "__builtin_popcount") {
    if (!node.arguments.empty()) {
        dispatch(node.arguments[0].get());
        emit("popcnt %eax, %eax");
    }
    return;
}
```

## What Works

- `__builtin_expect(expr, expected)` evaluates `expr` and leaves the
  result in `%rax`. The `expected` hint is ignored.
- `__builtin_offsetof(type, member)` returns the byte offset as an
  integer literal.
- `__builtin_popcount(x)` evaluates `x` (truncated to 32 bits) and
  emits the `popcnt` instruction.

## Limitations

- **No actual branch prediction** — `__builtin_expect` is lowered
  to its first argument; the optimizer would normally reorder the
  comparison, but there is no such pass.
- **`__builtin_popcount` truncates to 32 bits** — the value in
  `%rax` is first moved to `%eax` (implicit zero-extension) before
  the `popcnt` runs. There is no `__builtin_popcountll` (64-bit)
  support.
- **Many builtins not implemented** — `__builtin_constant_p`,
  `__builtin_types_compatible_p`, `__builtin_trap`,
  `__builtin_memcpy`, `__builtin_va_*`, etc.
- **`__builtin_offsetof` does not handle parenthesised types** like
  `__builtin_offsetof(struct Foo, field.subfield)` — only flat
  member names.

## Example

```c
int main() {
    int x = __builtin_expect(1, 1);     // returns 1
    int y = __builtin_popcount(255);    // returns 8
    return x + y;                       // returns 9
}
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Lexer entries | `src/lexer.cpp:152-155` | Map `__builtin_*` to `IDENTIFIER` |
| `__builtin_expect` | `src/codegen.cpp:1204-1210` | Evaluates the first argument |
| `__builtin_offsetof` | `src/codegen.cpp:1212-1229` | Calls `get_field_offset` and emits the result |
| `__builtin_popcount` | `src/codegen.cpp:1231-1238` | Emits `popcnt %eax, %eax` |
| `get_field_offset` | `src/codegen.h:185` | Computes struct-member offsets at compile time |
