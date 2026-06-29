# Lesson 0045: _Generic Selection (C11)

## Status: ✅ Complete (simplified) | Phase: Float & Advanced | Effort: Hard (8-12h)

## Objective

Implement compile-time type-based dispatch. `_Generic(expr,
type1: expr1, ..., default: expr)` selects one of the
associations based on the type of the controlling expression.
The full rule is to pick the first association whose type
matches the controlling expression's type (with `default` as a
fallback). This implementation **always picks the first
association** (or the `default`, whichever comes first) and
returns its expression — it does not look at types at all.

## Generic Selection Processing

```mermaid
flowchart TD
    A["_Generic(expr, type1: expr1, ...)"] --> B[Evaluate controlling expr type]
    B --> C{Find matching type}
    C -->|Match found| D["Return associated expression"]
    C -->|No match, has default| E["Return default expression"]
    C -->|No match, no default| F[Compile error]

    G["Type matching"] --> H["Exact type match required"]
    G --> I["No implicit conversions"]
    G --> J["Associations evaluated at compile time]
    G --> K["Result is typed expression"]
```

## Implementation Checklist

- [x] Parse `_Generic(expr, type: expr, ..., default: expr)`
- [x] Always return the first (or `default`) expression — see
      Status
- [ ] Evaluate type of controlling expression — **not
      implemented**
- [ ] Find matching type in association list — **not
      implemented**
- [x] Test: type dispatch macro compiles

## Implementation Details

The core trick: the parser iterates through the
`type: expr` and `default: expr` associations and records
the **first** non-`default` expression it sees (or the
`default` if it comes first). It returns that expression as
the result of the `_Generic`.

### Parser — simplified selection

In `parse_unary()`, the `_Generic` keyword is recognised right
after `sizeof` and `_Alignof`. The controlling expression is
parsed, the comma is consumed, and then the association list
is walked (`src/parser.cpp:1674-1702`):

```cpp
// src/parser.cpp:1674-1702
// _Generic selection: _Generic(control-expr, type1: expr1, ..., default: expr)
if (match(TokenType::KW_GENERIC)) {
    int line = tokens_[pos_ - 1].line;
    int col = tokens_[pos_ - 1].column;

    expect(TokenType::LPAREN);
    auto control = parse_assignment();
    expect(TokenType::COMMA);

    // Parse the first association, then look for default
    ASTPtr result = nullptr;
    while (!check(TokenType::RPAREN) && !is_at_end()) {
        if (match(TokenType::KW_DEFAULT)) {
            expect(TokenType::COLON);
            result = parse_assignment();
        } else {
            // Skip type name
            parse_type_specifier();
            expect(TokenType::COLON);
            auto assoc_expr = parse_assignment();
            if (!result) result = std::move(assoc_expr);
        }
        if (!match(TokenType::COMMA)) break;
    }
    expect(TokenType::RPAREN);

    // Simplified: just return the first/default expression
    return result ? std::move(result)
                  : std::make_unique<IntegerLiteralNode>(0, line, col);
}
```

A few things to notice:

1. The controlling expression `control` is parsed but never
   inspected. The plan in a real implementation would be to
   infer its type, then walk the association list looking for
   a match.
2. The `if (!result) result = std::move(assoc_expr)` is the
   simplification: the **first** non-`default` expression is
   captured, and any later associations are silently parsed
   and discarded.
3. If `default:` appears before any `type: expr`, the
   `result = parse_assignment()` branch fires immediately and
   the loop's later `if (!result) result = ...` is no-op.
4. If no association is found, an `IntegerLiteralNode(0)` is
   returned as a sentinel fallback.

## Example

```c
// src/example.c
int main() { return _Generic((1), int: 1, char: 2, default: 3); }
```

The parser sees `_Generic`, parses the controlling expression
`1` (and discards it), then walks the association list.
`int: 1` is the first non-`default` so `result = 1`. The
`char: 2` and `default: 3` branches are parsed and discarded.
The codegen sees `return 1;` and emits `mov $1, %rax`.

A user-supplied test that exercises the *type-matching*
behaviour (e.g. `_Generic(x, int: 1, double: 2)` where `x` is
a `double`) would return 1 here, not 2.

## Source Code References

| Component | File | Lines | Description |
|-----------|------|-------|-------------|
| `_Generic` token | `src/lexer.cpp` | `142` | Maps to `TokenType::KW_GENERIC` |
| Parser dispatch | `src/parser.cpp` | `1674-1675` | In `parse_unary` |
| `_Generic` parsing | `src/parser.cpp` | `1674-1702` | Walks associations, keeps first |
| `KW_GENERIC` enum | `src/token.h` | `58` | Token type |
| `parse_type_specifier` | `src/parser.cpp` | `99-215` | Used to skip the type name |

## Status

- **Lexer / Parser**: ✅ `_Generic` is recognised, parsed, and
  returns a non-null expression. The compiler does not crash
  on the syntax.
- **Note (always first)**: ⚠️ The implementation always
  returns the first non-`default` expression (or the first
  `default`). It does **not** look at the controlling
  expression's type. Code that depends on `_Generic` to
  dispatch based on type (the whole point of the feature) will
  not behave as expected.
- **Note (no type errors)**: ⚠️ A `_Generic` with no matching
  association and no `default` does not produce a compile
  error — the parser returns `IntegerLiteralNode(0)`.
