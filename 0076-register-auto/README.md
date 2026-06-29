# Lesson 0076: `register` and `auto` Storage Class

## Status: ✅ Complete | Phase: GCC Extensions | Effort: Easy

## Objective

Support the `register` and `auto` storage class specifiers. Both
are hints from the programmer; the compiler is free to ignore them.

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

Both keywords are added to the lexer's keyword map and token type
table. The parser recognises them in the type-specifier chain
(`parse_type_specifier`) and appends them to the type-name string.
The codegen then strips them before computing the variable's
underlying type, so they map to ordinary `int` storage on the
stack.

```cpp
// src/parser.cpp:103
while (true) {
    if (match(TokenType::KW_CONST))      { result += "const "; }
    else if (match(TokenType::KW_VOLATILE))  { result += "volatile "; }
    else if (match(TokenType::KW_STATIC))    { result += "static "; }
    else if (match(TokenType::KW_INLINE))    { result += "inline "; }
    else if (match(TokenType::KW_REGISTER))  { result += "register "; }
    else if (match(TokenType::KW_AUTO))      { result += "auto "; }
    // ... more qualifiers
    else break;
}
```

The codegen strips the storage-class keywords before doing any type
arithmetic:

```cpp
// src/codegen.cpp:2109
std::string CodeGenerator::get_struct_name(const std::string& type_name) {
    std::string s = type_name;
    // strip qualifiers
    auto strip = [&](const std::string& kw) {
        size_t p;
        while ((p = s.find(kw + " ")) != std::string::npos) s.erase(p, kw.size() + 1);
    };
    strip("const"); strip("volatile"); strip("static"); strip("extern");
    strip("register"); strip("auto"); strip("inline"); strip("restrict");
    if (s.substr(0, 7) == "struct ") s = s.substr(7);
    return s;
}
```

## What Works

- `register int x;` and `auto int x;` are accepted in any position
  in the type specifier (between and after storage classes).
- The variables occupy stack space just like an unadorned local.
- Combinations such as `static register int x;` are accepted (and
  `static` wins, since it is the stricter property).

## Limitations

- No actual register allocation — the codegen does not honor the
  `register` hint to put a variable in a register. (Register
  allocation is a separate lesson, 0068.)
- `&register_var` is not rejected by the compiler; real C requires
  a diagnostic.

## Example

```c
int main() {
    register int fast_var = 42;
    auto int auto_var = 10;
    return fast_var + auto_var;  // returns 52
}
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| `KW_REGISTER` | `src/token.h:52` | Token type |
| `KW_AUTO` | `src/token.h:53` | Token type |
| Lexer keywords | `src/lexer.cpp:135-136` | `"register"` → `KW_REGISTER`, `"auto"` → `KW_AUTO` |
| Token name | `src/lexer.cpp:41-42` | Maps to `"register"` / `"auto"` strings |
| Type specifier | `src/parser.cpp:112-115` | Appends `register ` / `auto ` to the type string |
| Codegen strip | `src/codegen.cpp:2116-2117` | Strips the keywords before type arithmetic |
