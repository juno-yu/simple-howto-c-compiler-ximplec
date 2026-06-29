# Lesson 3003: Empty Structures (C23)

## Status: ✅ Complete (parses, size 0) | Standard: C23 | Effort: Easy

## Objective

Allow empty struct and union declarations.

## How It Works

The struct parser walks the `{ ... }` body, returning a `StructDeclNode` with an empty `fields` vector. The struct codegen then records an empty layout:

```cpp
// src/parser.cpp:812-838 — parse_struct_decl
auto struct_decl = std::make_unique<StructDeclNode>(name, ...);
expect(TokenType::LBRACE);
while (!check(TokenType::RBRACE) && !is_at_end()) { /* ... */ }
expect(TokenType::RBRACE);
expect(TokenType::SEMICOLON);
return std::move(struct_decl);
```

```cpp
// src/codegen.cpp:600-616 — visit(StructDeclNode)
void CodeGenerator::visit(StructDeclNode& node) {
    std::vector<FieldInfo> fields;
    int offset = 0;
    for (auto& field_ast : node.fields) { /* ... */ }
    struct_layouts_[node.name] = fields;  // empty layout is recorded
}
```

```cpp
// src/codegen.cpp:2093-2099 — get_struct_size
int CodeGenerator::get_struct_size(const std::string& name) {
    if (!struct_layouts_.count(name)) return 0;
    const auto& fields = struct_layouts_[name];
    if (fields.empty()) return 0;          // ← returns 0, not 1
    const auto& last = fields.back();
    return last.offset + last.size;
}
```

Note: simplecc reports `sizeof(struct Empty)` as **0**, not 1 (C23 specifies 1 byte). The bundled example in `3003-c23-empty-structs/src/example.c` only declares a local `struct Empty e;` and returns `0` — `sizeof` is not called.

## Syntax

```c
struct empty { };  // simplecc: parses, size 0
union empty { };   // simplecc: parses, size 0
```

## Example

```c
struct Empty {};
int main() {
    struct Empty e;
    return 0;
}
```

## Limitations

- `sizeof(struct Empty)` returns `0`, not the C23-mandated `1`.
- No "no fields" diagnostic is emitted — the empty body is silently accepted.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|---|
| Struct decl parser | `src/parser.cpp:812-838` | Walks zero or more fields |
| Struct layout | `src/codegen.cpp:600-616` | Records an empty field list |
| Size query | `src/codegen.cpp:2093-2099` | Returns 0 for empty layouts |
