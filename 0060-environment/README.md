# Lesson 0060: Environment Variables

## Status: ✅ Complete | Phase: Stdlib Tier B | Effort: Easy

## Objective

Enable programs to access and modify environment variables using the
standard C functions (`getenv`, `setenv`, `unsetenv`) by declaring the
functions as `extern` and letting the linker resolve the calls against
the system C library.

## Implementation

`simplecc` does not ship a bundled `getenv`/`setenv` declaration in
`lib/stdlib.h` — the user declares the prototypes they need:

```c
// User-declared prototypes
char *getenv(const char *name);
int   setenv(const char *name, const char *value, int overwrite);
int   unsetenv(const char *name);
extern char **environ;
```

The codegen produces a `call getenv` (etc.) with the name argument
loaded into `%rdi` via `lea .Lstr_N(%rip), %rax` for string literals
(`src/codegen.cpp:1534-1541`).

## What Works

- `getenv` / `setenv` / `unsetenv` when declared as `extern`.
- Returning a `char *` from `getenv` — the pointer is left in `%rax`
  and can be assigned to a `char *` local.
- Reading the `environ` global if the user declares it.

## Limitations

- No bundled declaration. The user must declare the prototype.
- `setenv` / `unsetenv` are POSIX, not standard C. They are not
  always available on every libc; on Windows, for example, they
  are missing.
- The compiler does not provide a built-in `environ` symbol — the
  user must declare `extern char **environ;` themselves.

## Example

```c
char *getenv(char *name);
int main() {
    char *home = getenv("HOME");
    return 0;
}
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| `parse_declaration` | `src/parser.cpp:300-336` | `extern` declarations produce a no-body `FunctionDeclNode` |
| Pointer-type parsing | `src/parser.cpp:255-262` | `char *` parameter and return types |
| Forward-decl emit | `src/codegen.cpp:305-309` | Skips emission when `body == nullptr` |
| Function call codegen | `src/codegen.cpp:1288-1364` | System V ABI register assignment |
| String literal codegen | `src/codegen.cpp:1534-1541` | `lea .Lstr_N(%rip), %rax` for the `name` arg |
