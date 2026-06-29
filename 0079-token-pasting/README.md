# Lesson 0079: Token Pasting (`##`) and Stringification (`#`)

## Status: ✅ Complete | Phase: GCC Extensions | Effort: Easy

## Objective

Support the two macro operators that operate on tokens at expansion
time:

- **`##`** (token pasting): concatenates two adjacent tokens into
  one.
- **`#`** (stringification): converts a macro argument into a
  string literal.

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

```cpp
// src/preprocessor.cpp:346
// Handle # stringification in body
for (size_t i = 0; i < macro.params.size() && i < expanded_args.size(); i++) {
    std::string hash_pattern = "#" + macro.params[i];
    size_t pos = 0;
    while ((pos = result.find(hash_pattern, pos)) != std::string::npos) {
        // Skip if this # is part of ## (preceded by another #)
        if (pos > 0 && result[pos - 1] == '#') {
            pos += hash_pattern.size();
            continue;
        }
        result.replace(pos, hash_pattern.size(), "\"" + expanded_args[i] + "\"");
        pos += expanded_args[i].size() + 2;
    }
}

// Handle ## token pasting in body
for (size_t i = 0; i < macro.params.size() && i < expanded_args.size(); i++) {
    std::string paste_pattern = macro.params[i] + "##";
    size_t pos = 0;
    while ((pos = result.find(paste_pattern, pos)) != std::string::npos) {
        size_t after = pos + paste_pattern.size();
        while (after < result.size() && result[after] == ' ') after++;
        size_t end = after;
        while (end < result.size() && (std::isalnum(result[end]) || result[end] == '_')) end++;
        std::string next_token = result.substr(after, end - after);
        result.replace(pos, end - pos, expanded_args[i] + next_token);
        pos += expanded_args[i].size() + next_token.size();
    }
}
```

The `#` handler wraps the expanded argument in `"..."` (no escape
processing — embedded quotes are not currently escaped). The `##`
handler concatenates the parameter's expanded text with the
following identifier in the body.

## What Works

- `a ## b` token pasting — the surrounding tokens are concatenated
  into one.
- `#x` stringification — the expanded argument is wrapped in
  `"..."`.
- Order independence: `##` and `#` work whether the macro body uses
  them before or after parameter substitution.
- Interaction: `a ## b` where the `a` is a parameter that gets
  pasted to `b` works correctly.

## Limitations

- **Stringification does not escape embedded quotes or backslashes.**
  `#define Q(x) #x; Q(hello "world")` would produce
  `"hello "world""` — a malformed string literal. Real C would
  produce `"hello \"world\""`.
- **`#` does not respect the original (unexpanded) token text** —
  it operates on the *expanded* argument. Real C stringification
  operates on the unexpanded tokens (so `#define INC(x) #x; INC(1+1)`
  produces `"1+1"`, not `"2"`).
- **Token pasting in unusual positions** (e.g. `a ## 1` to produce
  a numeric literal) is not explicitly tested.

## Example

```c
#define CONCAT(a, b) a##b
#define STRINGIFY(x) #x
int main() {
    int xy = 42;
    return CONCAT(x, y);     // expands to: return xy;  → 42
}
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Stringification | `src/preprocessor.cpp:346-359` | Wraps argument in `"..."` |
| Token pasting | `src/preprocessor.cpp:361-374` | Concatenates parameter with following identifier |
| `##` in `__VA_ARGS__` | `src/preprocessor.cpp:351-354` | Skips `#` that is part of `##` |
| `__VA_ARGS__` substitute | `src/preprocessor.cpp:376-381` | Replaces each occurrence in the body |
