# Lesson 0073: Compile Compiler (Parser) — Self-Hosting Step

## Status: ⚠️ Partial | Phase: Self-Hosting | Effort: Hard

## Status

Placeholder lesson in the self-hosting roadmap. See [Lesson 0072](../0072-compile-compiler-1/README.md) for context.

## Limitation

The compiler is in C++17 with STL dependencies. A C port is required before any self-hosting milestone can be reached.

## Example

The example program is a placeholder:

```c
int main() { return 0; }
```

## Future Work

1. Port simplecc to C11 (lesson 0071 prerequisite).
2. Use the C port to compile its own parser source.
3. Verify the resulting binary matches a reference build.
