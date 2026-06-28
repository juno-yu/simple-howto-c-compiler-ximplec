# Lesson 0076: `register` and `auto` Storage Class

## Status: ✅ Complete | Phase: GCC Extensions | Effort: Easy

## Objective

Support the `register` and `auto` storage class specifiers. Both are hints from the programmer; the compiler is free to ignore them.

## Syntax

```c
register int fast_var = 42;  // hint: prefer register
auto int auto_var = 10;       // explicit auto (default for locals)
```

## Semantics

| Specifier | C Standard | Behavior |
|-----------|-----------|----------|
| `auto`    | C89       | Default for local variables. Variable has automatic storage duration. |
| `register`| C89       | Hint that the variable is heavily used. Address-of (`&`) forbidden in strict C. |

## Implementation

Both keywords are lexed and added to the keyword map. The parser recognizes them in the type-specifier chain (`parse_type_specifier`) and concatenates them into the type name string. The codegen treats them identically to a regular local — they occupy stack space just like an unadorned local would.

## Limitations

- No actual register allocation — the codegen does not honor the `register` hint to put a variable in a register.
- `&register_var` is not rejected by the compiler (real C requires a diagnostic).

## Example

```c
int main() {
    register int fast_var = 42;
    auto int auto_var = 10;
    return fast_var + auto_var;  // returns 52
}
```

## Source Code References

| Component | File | Description |
|-----------|------|-------------|
| Lexer keyword | `src/lexer.cpp` | `register`, `auto` mapped to `KW_REGISTER`, `KW_AUTO` |
| Type specifier | `src/parser.cpp` (`parse_type_specifier`) | Recognizes and consumes both qualifiers |
| Codegen | `src/codegen.cpp` | Local-variable storage, no special treatment |
