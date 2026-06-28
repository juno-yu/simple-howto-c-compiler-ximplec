# Lesson 0086: Nested Functions (GCC Extension)

## Status: ⚠️ Partial | Phase: GCC Extensions | Tests: Basic

## Objective

Support GCC-style nested functions — functions defined inside other functions that can access the enclosing scope's variables.

## Current Example

```c
int main() {
    int offset = 10;
    int add_offset(int x) {
        return x + offset;  // captures 'offset' from enclosing scope
    }
    return add_offset(5);   // returns 15
}
```

## Implementation Status

### What Works Now

| Feature | Status | Notes |
|---------|--------|-------|
| Function declaration inside block | ✅ | Parser detects `type name(...)` inside `{}` |
| Nested function body parsing | ✅ | Recursive descent handles nested blocks |
| Separate function generation | ⚠️ | Generated as top-level, but captures broken |
| Simple calls (no capture) | ✅ | `int f(int x) { return x; }` inside main works |

### What Doesn't Work

| Feature | Status | Notes |
|---------|--------|-------|
| Variable capture from enclosing scope | ❌ | `offset` accessed via wrong addressing |
| Trampoline generation | ❌ | GCC uses trampolines on stack for closures |
| Multiple nesting levels | ❌ | Only one level of nesting |
| Nested function pointers | ❌ | Can't pass nested function as callback |
| `auto` nested functions | ❌ | GCC allows `auto` keyword for nested functions |

## Why Nested Functions Are Hard

### The Problem

When `add_offset` is called, it needs access to `offset` which lives on `main`'s stack frame. But `add_offset` is a separate function — it doesn't have a reference to `main`'s frame.

```
main's stack frame:
┌─────────────────┐
│ return address   │
│ saved %rbp       │
│ offset = 10      │  ← add_offset needs this
│ ...              │
└─────────────────┘

add_offset's stack frame:
┌─────────────────┐
│ return address   │
│ saved %rbp       │  ← this points to add_offset's frame, not main's
│ x = 5            │
└─────────────────┘
```

### GCC's Solution: Trampolines

GCC implements nested functions using **trampolines** — small executable code blocks allocated on the stack:

```
1. When address of nested function is taken, GCC allocates a trampoline on stack
2. Trampoline contains:
   - Load frame pointer of enclosing function
   - Jump to actual nested function body
3. Nested function receives enclosing frame pointer as hidden parameter
```

```
Trampoline (on stack):
┌──────────────────────────────┐
│ mov $enclosing_frame, %r10   │  ← 10 bytes
│ jmp nested_function_body     │  ← 5 bytes
└──────────────────────────────┘
```

### Why We Don't Implement Trampolines

1. **Stack executable code** — Modern security (NX bit) prevents executing code on stack
2. **Complexity** — Requires mprotect() to make stack executable, or mmap() for trampolines
3. **GCC-specific** — Not standard C, rarely used in practice
4. **Better alternatives** — `void*` context parameters, closures in C++11 lambdas

## Alternative Approaches

### 1. Hidden Parameter (Our Current Approach)

Pass a pointer to the enclosing scope as a hidden first parameter:

```c
// What we generate:
void main.add_offset(int scope_ptr, int x) {
    int offset = *(int*)(scope_ptr + 8);  // offset is at scope+8
    return x + offset;
}

int main() {
    int offset = 10;
    // Call with scope pointer:
    main.add_offset(&offset, 5);
}
```

**Limitation:** Requires knowing exact stack layout at compile time.

### 2. Static Link (Like Pascal/Ada)

Use a static chain pointer that links to the enclosing scope:

```c
// Nested function receives static_link as hidden parameter
int add_offset(int x) {
    // static_link points to main's frame
    return x + static_link->offset;
}
```

**Limitation:** Requires runtime support for chain traversal.

### 3. Context Parameter (Manual)

Let the programmer pass context explicitly:

```c
int add_offset(void *ctx, int x) {
    int offset = *(int*)ctx;
    return x + offset;
}

int main() {
    int offset = 10;
    return add_offset(&offset, 5);  // manual context passing
}
```

**Limitation:** Not transparent to the programmer.

## References

- [GCC Nested Functions](https://gcc.gnu.org/onlinedocs/gcc/Nested-Functions.html)
- [Trampolines (Wikipedia)](https://en.wikipedia.org/wiki/Trampoline_(computer_programming))
- [CompilingNested.pdf](https://www.cs.indiana.edu/~kepapke/CS536/KePapke-NEsted.pdf)

## Testing

```bash
# Simple nested function (no capture) - should work
int main() {
    int add(int x) { return x + 1; }
    return add(5);  // returns 6
}

# Nested function with capture - currently broken
int main() {
    int y = 10;
    int add(int x) { return x + y; }
    return add(5);  // should return 15, currently returns wrong value
}
```
