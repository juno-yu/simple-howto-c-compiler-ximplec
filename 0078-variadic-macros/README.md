# Lesson 0078: Variadic Macros

## Status: ✅ Complete | Phase: GCC Extensions | Effort: Easy

## Objective

Support C99 variadic macros that accept a variable number of
arguments via `...` and `__VA_ARGS__`.

## Syntax

```c
#define SUM(...)      __VA_ARGS__
#define LOG(msg, ...) fprintf(stderr, msg, __VA_ARGS__)
#define DEBUG(fmt, args...) printf(fmt, args)
```

## Semantics

- `...` in the parameter list matches zero or more arguments.
- `__VA_ARGS__` expands to the comma-separated list of variadic
  arguments.
- C20 adds `__VA_OPT__(content)` to expand `content` only when
  `__VA_ARGS__` is non-empty (not implemented in this project).

## Implementation

The preprocessor:

1. Detects `(` immediately after a `#define` identifier and parses
   a parameter list.
2. Recognizes `...` as a special parameter and marks the macro as
   variadic.
3. During expansion, `__VA_ARGS__` is replaced with the matched
   arguments (empty if none).
4. The expansion is rescanned for nested macros.

The variadic flag is set when the last parameter of a function-like
macro is literally `...`:

```cpp
// src/preprocessor.cpp:419
std::string params_str = args.substr(space + 1, close - space - 1);
std::vector<std::string> params = split_args(params_str);
bool is_variadic = false;

// Check for variadic ... at end of params
if (!params.empty() && params.back() == "...") {
    is_variadic = true;
    params.pop_back();
}

std::string body = args.substr(close + 1);
// ... body trimming

Macro macro(name, body, true, is_variadic);
macro.params = params;
macros_[name] = macro;
```

At expansion time, the preprocessor collects the extra arguments
beyond the regular parameter list and substitutes them where
`__VA_ARGS__` appears in the body:

```cpp
// src/preprocessor.cpp:337
// Handle variadic macros: __VA_ARGS__ gets the remaining args
std::string va_args_str;
if (macro.is_variadic && expanded_args.size() > macro.params.size()) {
    for (size_t i = macro.params.size(); i < expanded_args.size(); i++) {
        if (i > macro.params.size()) va_args_str += ", ";
        va_args_str += expanded_args[i];
    }
}
// ... later ...
// Replace __VA_ARGS__
size_t va_pos = result.find("__VA_ARGS__");
while (va_pos != std::string::npos) {
    result.replace(va_pos, 11, va_args_str);
    va_pos = result.find("__VA_ARGS__", va_pos + va_args_str.size());
}
```

## What Works

- `...` as the last parameter of a function-like macro.
- `__VA_ARGS__` substitution with comma-separated variadic args.
- GNU-style named variadic args (`args...`) — the preprocessor
  handles the parameter-list parsing generically and stores the
  named portion as the final parameter.

## Limitations

- `__VA_OPT__` is not implemented — C20 feature.
- An empty `__VA_ARGS__` in C99 strict mode would produce a stray
  comma in some constructs (`fprintf(stderr, msg, )`); the
  preprocessor does not special-case this.

## Example

```c
#define SUM(...) __VA_ARGS__
#define LOG(msg, ...) msg
int main() {
    int x = SUM(1 + 2 + 3);
    return x;     // returns 6
}
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Macro parsing | `src/preprocessor.cpp:419-435` | Detects `...` and sets `is_variadic` |
| `is_variadic` field | `src/preprocessor.h:14-18` | Stored on the `Macro` struct |
| `__VA_ARGS__` collection | `src/preprocessor.cpp:337-344` | Joins extra args with `, ` |
| `__VA_ARGS__` substitution | `src/preprocessor.cpp:376-381` | Replaces each occurrence in the body |
| Rescan | `src/preprocessor.cpp:331-335` | Re-expands macro args for nested expansion |
