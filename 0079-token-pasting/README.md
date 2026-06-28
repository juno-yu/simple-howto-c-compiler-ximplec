# Lesson 0079: Token Pasting (`##`) and Stringification (`#`)

## Status: ✅ Complete | Phase: GCC Extensions | Effort: Easy

## Objective

Support the two macro operators that operate on tokens at expansion time:
- **`##`** (token pasting): concatenates two adjacent tokens into one.
- **`#`** (stringification): converts a macro argument into a string literal.

## Syntax

```c
#define CONCAT(a, b)   a##b
#define STRINGIFY(x)   #x
#define MAKE_STR(x)    #x
```

## Semantics

| Operator | Effect |
|----------|--------|
| `a ## b` | Produces a single token `ab` (concatenation before lexing). |
| `#x`     | Produces a string literal `"x"` with embedded quotes escaped. |

## Implementation

The preprocessor scans each macro body for `##` and `#`:
- `##`: removes the operator and concatenates the surrounding tokens (after first expanding them if they are parameters).
- `#`: takes the literal text of the next token (or balanced parenthesized group) and wraps it in `"..."`, escaping `\` and `"`.

## Example

```c
#define CONCAT(a, b)   a##b
#define STRINGIFY(x)   #x
int main() {
    int xy = 42;
    return CONCAT(x, y);     // expands to: int xy; return xy;  → 42
}
```

## Source Code References

| Component | File | Description |
|-----------|------|-------------|
| Token pasting | `src/preprocessor.cpp` | Detects `##`, concatenates tokens |
| Stringification | `src/preprocessor.cpp` | Detects `#`, wraps argument in quotes |
| Argument capture | `src/preprocessor.cpp` | Captures raw tokens before expansion for `#` |
