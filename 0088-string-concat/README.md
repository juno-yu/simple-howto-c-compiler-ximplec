# Lesson 0088: String Literal Concatenation

## Status: ✅ Complete | Phase: Advanced Types | Effort: Easy

## Objective

Support adjacent string literal merging: `"hel" "lo"` → `"hello"`.

## How It Works

The preprocessor merges adjacent string literals in a final pass (C translation phase 6). Whitespace and comments between strings are skipped.

```cpp
// src/preprocessor.cpp:160-198
// String literal concatenation: merge adjacent "..." "..." tokens
std::string result;
size_t i = 0;
while (i < output.size()) {
    if (output[i] != '"') { result += output[i]; i++; continue; }
    std::string str;
    while (i < output.size() && output[i] != '\n') {
        if (output[i] == '\\' && i + 1 < output.size()) {
            str += output[i]; str += output[i+1]; i += 2; continue;
        }
        str += output[i]; i++;
        if (!str.empty() && str.back() == '"') break;
    }
    // ... skip whitespace, then if next char is '"', merge into str ...
    result += str;
}
output = result;
```

```c
char *s = "hel" "lo";   // becomes "hello"
char *msg = "line 1\n"
            "line 2\n"; // becomes "line 1\nline 2\n"
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| String concat | `src/preprocessor.cpp:160-198` | Final pass merges adjacent `"..." "..."` |
