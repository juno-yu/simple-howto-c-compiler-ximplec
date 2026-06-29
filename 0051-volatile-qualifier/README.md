# Lesson 0051: `volatile` Qualifier

## Status: ✅ Complete | Phase: System Integration | Effort: Easy

## Objective

Recognize the `volatile` type qualifier. The compiler accepts it in the
type-specifier chain and on pointer types, and propagates the qualifier
into the type string. The codegen does not currently treat `volatile`
accesses differently from regular accesses — every load/store is already
emitted as a concrete `mov` into or out of memory, which preserves
volatile semantics at the assembly level.

## Syntax

```c
volatile int status;
const volatile int hardware_register = 0x1234;
int *volatile ptr;            // volatile pointer to int
volatile int *p;              // pointer to volatile int
```

## Implementation

The keyword is recognised by the lexer (`src/lexer.cpp:132`) and added
to the type-specifier chain. `parse_type_specifier` consumes it and
appends the literal text to the type string:

```cpp
// src/parser.cpp:106
} else if (match(TokenType::KW_VOLATILE)) {
    result += "volatile ";
}
```

`is_type_specifier` (src/parser.cpp:58-97) recognises `KW_VOLATILE` as
the start of a type so that declarations like
`volatile int x = 0;` are parsed correctly.

The codegen strips `volatile` from the type string before computing
sizes and struct names:

```cpp
// src/codegen.cpp:2116
strip("const"); strip("volatile"); strip("static"); strip("extern");
strip("register"); strip("auto"); strip("inline"); strip("restrict");
```

This is a deliberate "no-op" treatment: every load of a local variable
in the current codegen is emitted as a real `mov` from its stack slot
into `%rax`, and every store is a real `mov` to the slot. Volatile
semantics are therefore preserved by accident — the compiler already
never caches a value in a register across statements.

## What Works

- `volatile` is accepted in any position in the type specifier.
- `volatile` is accepted in pointer chains (`volatile int *`, `int *volatile`).
- `const volatile` combinations are accepted.
- Loads and stores of `volatile` objects are emitted as concrete
  memory-touching instructions (existing codegen behavior).

## Limitations

- No formal "every read is a memory load" guarantee is documented in
  the codegen — it just happens to be true today. A future pass that
  caches values in registers across statements would need to special-case
  volatile accesses.
- No diagnostic is produced for code that takes the address of a
  `volatile` object and aliases a non-volatile pointer to it.

## Example

```c
int main() {
    volatile int x = 10;
    x++;
    return x;       // returns 11
}
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| `KW_VOLATILE` token | `src/token.h:49` | Token type definition |
| `token_name` | `src/lexer.cpp:38` | Maps `KW_VOLATILE` to `"volatile"` |
| Keyword map | `src/lexer.cpp:132` | `"volatile"` → `KW_VOLATILE` |
| `is_type_specifier` | `src/parser.cpp:73` | Recognises `KW_VOLATILE` |
| Qualifier parse | `src/parser.cpp:106-107` | Appends `volatile ` to the type string |
| Type sanitization | `src/codegen.cpp:2116` | Strips `volatile` before computing size/name |
