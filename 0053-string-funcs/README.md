# Lesson 0053: String Functions

## Status: ✅ Complete | Phase: Stdlib Tier A | Effort: Easy (4-6h)

## Objective

Implement basic string manipulation functions.

## String Functions Overview

```mermaid
flowchart TD
    A[String Functions] --> B[strlen]
    A --> C[strcmp]
    A --> D[strcpy]
    A --> E[	strcat]
    A --> F[memcpy]
    A --> G[memset]
    A --> H[memmove]

    B -->|"strlen(s)"| I[Count bytes until \0]
    C -->|"strcmp(a,b)"| J[Byte-by-byte comparison]
    D -->|"strcpy(dst,src)"| K[Copy bytes until \0]
    E -->|"strcat(dst,src)"| L[Append src to dst]
    F -->|"memcpy(dst,src,n)"| M[Copy n bytes]
    G -->|"memset(dst,c,n)"| N[Fill n bytes with c]
    H -->|"memmove(dst,src,n)"| O[Copy n bytes, handles overlap]
```

## String Memory Layout

```mermaid
flowchart LR
    A["char *s = \"hello\""] --> B[.rodata section]
    B --> C["h\0 | e\0 | l\0 | l\0 | o\0"]
    D["char buf[6]"] --> E[Stack allocation]
    E --> F["h | e | l | l | o | \0"]

    G[Pointer s] --> C
    H[Array buf] --> E
```

## Functions

| Function | Complexity |
|----------|------------|
| `strlen(s)` | Easy |
| `strcmp(a,b)` | Easy |
| `strcpy(dst,src)` | Easy |
| `strcat(dst,src)` | Easy |
| `memcpy(dst,src,n)` | Easy |
| `memset(dst,c,n)` | Easy |
| `memmove(dst,src,n)` | Medium |

## Implementation Checklist

- [ ] Implement strlen: count until \0
- [ ] Implement strcmp: byte comparison
- [ ] Implement strcpy: copy bytes
- [ ] Implement memcpy: copy n bytes
- [ ] Implement memset: fill n bytes
- [ ] Test: `strlen("hello")` → 5
- [ ] Test: `strcmp("abc", "abd")` → negative

## Implementation Details

String functions are declared as `extern` and linked to the C library at link time. The compiler handles string literal codegen by emitting `.asciz` directives in the `.rodata` section and generates `call` instructions for string function invocations.

| Component | File | Line | Description |
|-----------|------|------|-------------|
| Extern parse | `src/parser.cpp` | 218-248 | Parses `extern` function declarations |
| String literal | `src/codegen.cpp` | 929-936 | Emits string to `.rodata` with `lea` for address |
| `.rodata` emit | `src/codegen.cpp` | 46-53 | Collects and emits string literals section |
| `.asciz` output | `src/codegen.cpp` | 51 | Outputs `.asciz` for null-terminated strings |
| Func call | `src/codegen.cpp` | 838-854 | Generates `call` for strlen, strcmp, etc. |
| Index access | `src/codegen.cpp` | 856-897 | Array/string indexing with element size scaling |
| Test coverage | `tests/test_string_funcs.cpp` | 1-99 | Tests strlen call, string literal assignment |
