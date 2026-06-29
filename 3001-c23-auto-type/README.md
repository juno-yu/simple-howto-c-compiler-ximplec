# Lesson 3001: auto Type Inference (C23)

## Status: ⚠️ Partial | Standard: C23 | Effort: Easy

## Objective

Infer variable type from initializer.

## How It Works

`auto` is a real keyword (`src/lexer.cpp:136`) and is consumed in `parse_type_specifier` as a storage-class qualifier:

```cpp
// src/lexer.cpp:136
{"auto", TokenType::KW_AUTO},
```

```cpp
// src/parser.cpp:114-115
} else if (match(TokenType::KW_AUTO)) {
    result += "auto ";
}
```

simplecc does **not** actually perform type inference from the initializer expression. The variable is declared with type-name string `"auto "` and the initializer drives the operand types in expressions. For `auto x = 42;` the result is the same as a normal local because the integer literal is lowered to a 4-byte `int` in the assembly.

## Syntax

```c
auto x = 42;        // emits int-sized assignment
auto y = 3.14;      // emits double-sized assignment
auto *p = &x;       // works
auto arr[3] = {1,2,3}; // ⚠️ does not infer the array size, see lesson 3011
```

## Example

```c
int main() {
    auto x = 42;
    auto y = 3.14;
    return x;  // returns 42
}
```

## What Works

| Pattern | Status |
|---------|--------|
| `auto x = 42;` | ✅ |
| `auto y = 3.14;` | ✅ |
| `auto *p = &x;` | ✅ |
| `auto y;` (no initializer) | ❌ — would need type-inference from `auto y;` itself, which is invalid in C23 |
| `auto arr[] = {1,2,3};` | ⚠️ Compiles but segfaults; size is not inferred |

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|---|
| Lexer | `src/lexer.cpp:136` | `auto` → `KW_AUTO` |
| Type-specifier recognition | `src/parser.cpp:79` | Listed in `is_type_specifier` |
| Qualifier string | `src/parser.cpp:114-115` | Appended to type |
