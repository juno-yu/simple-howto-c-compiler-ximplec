# Lesson 0088: String Literal Concatenation

## Status: ✅ Complete | Phase: Advanced Types | Effort: Easy

## Objective

Support adjacent string literal merging: `"hel" "lo"` → `"hello"`.

## How It Works

The preprocessor merges adjacent string literals in a final pass (C translation phase 6). Whitespace and comments between strings are skipped.

```c
char *s = "hel" "lo";   // becomes "hello"
char *msg = "line 1\n"
            "line 2\n"; // becomes "line 1\nline 2\n"
```

## Source Code References

| Component | File | Description |
|-----------|------|-------------|
| String concat | `src/preprocessor.cpp` | Final pass merges adjacent `"..." "..."` |
