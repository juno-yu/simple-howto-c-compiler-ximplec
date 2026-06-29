# Lesson 0091: Typedef for Function Pointers

## Status: ✅ Complete | Phase: Advanced Types | Effort: Easy

## Objective

Support `typedef int (*func_t)(int);` for function pointer types.

## How It Works

A function-pointer typedef is recognised by a `(*name)` declarator followed by a balanced parameter list. The parameter list is depth-counted and skipped; the alias is added to `typedef_names_` and the source type string is mangled to record the function-pointer shape.

The dedicated `parse_typedef_decl` (`src/parser.cpp:885-934`) handles file-scope typedefs:

```cpp
// src/parser.cpp:885-924
ASTPtr Parser::parse_typedef_decl() {
    std::string source_type = parse_type_specifier();
    std::string alias;
    // ...
    if (check(TokenType::IDENTIFIER)) {
        alias = peek().value;
        advance();
    } else if (match(TokenType::LPAREN)) {
        // Function pointer typedef: typedef int (*func_t)(int);
        if (match(TokenType::STAR)) {
            alias = peek().value; advance();
            expect(TokenType::RPAREN);
        } else {
            error("Expected typedef name");
            return nullptr;
        }
        // Skip the parameter list
        if (match(TokenType::LPAREN)) {
            int depth = 1;
            while (depth > 0 && !is_at_end()) { /* ... */ }
        }
        source_type = source_type + "(*)";
    }
    typedef_names_.insert(alias);
    // ...
}
```

An equivalent inline handler lives inside `parse_statement` (`src/parser.cpp:1072-1109`) for typedefs that appear inside a function body — this is the path the bundled example hits.

The alias is registered in `typedef_names_` so it can later appear wherever a type specifier is expected (`src/parser.cpp:94, 242`).

## Status

| Feature | Status |
|---------|--------|
| `typedef int (*func_t)(int)` | ✅ Aliased to `int(*)` and registered |
| Subsequent use as a type | ✅ Recognized in `is_type_specifier` |

## Example

```c
int main() {
    typedef int (*func_t)(int);
    return 0;
}
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Function-pointer typedef | `src/parser.cpp:896-924` | `(*name)(...)` declarator |
| Alias storage | `src/parser.h:91` | `typedef_names_` set |
| Type-name lookup | `src/parser.cpp:94, 242` | Reuse the alias as a type |
