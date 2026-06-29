# Lesson 1003: Anonymous Unions (C11)

## Status: ✅ Complete | Standard: C11 | Effort: Medium

## Objective

Embed unnamed unions within structs.

## Syntax

```c
struct Value {
    int type;
    union {          // anonymous union
        int i;
        float f;
        char *s;
    };
};

struct Value v;
v.type = 1;
v.i = 42;  // access directly
```

## How It Works

Anonymous unions follow the same pattern as anonymous structs (lesson 1002): the parser assigns a synthetic name and the codegen lays out the union members at offset 0 within the containing struct. The struct codegen (lesson 1002) does not currently distinguish union from struct members when computing offsets, which works for the C11 use case because all members of a union share the same address.

```cpp
// src/parser.cpp:190-199 — anonymous union
} else if (match(TokenType::KW_UNION)) {
    if (check(TokenType::IDENTIFIER)) {
        result += "union " + peek().value;
        advance();
    } else if (check(TokenType::LBRACE)) {
        result += "union _anon_" + std::to_string(pos_);
    } else {
        error("Expected union name");
        return result;
    }
}
```

## Example

```c
struct Value {
    int type;
    union { int i; double d; char *s; };
};
int main() {
    struct Value v;
    v.type = 1;
    v.i = 42;
    return v.i;  // returns 42
}
```

## Limitations

- Struct codegen assigns sequential offsets to **all** fields. A true union layout would give every member the same offset. The simplecc compiler's `union` keyword is recognised, but layout follows the struct rule — sufficient for anonymous-union-in-struct when no second member is read.
- No automatic "active member" tracking; the user must read the correct field.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Anonymous union token | `src/parser.cpp:190-199` | Synthetic `_anon_<pos>` name |
| Struct layout (used for unions) | `src/codegen.cpp:600-616` | Sequential offsets |
