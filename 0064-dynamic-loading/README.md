# Lesson 0064: Dynamic Loading

## Status: ✅ Complete | Phase: Stdlib Tier C | Effort: Hard

## Objective

Enable programs to load shared libraries at runtime using the POSIX
`dlopen` / `dlsym` / `dlclose` / `dlerror` functions, by declaring
the functions as `extern` and letting the linker resolve the calls
against `libdl` (which provides the dynamic-loader glue).

## Implementation

`simplecc` does not ship a bundled `<dlfcn.h>` header — the user
declares the prototypes they need:

```c
// User-declared prototypes
void *dlopen(const char *filename, int flags);
void *dlsym(void *handle, const char *symbol);
int   dlclose(void *handle);
char *dlerror(void);

// Common flag values (also user-declared)
#define RTLD_LAZY  1
#define RTLD_NOW   2
#define RTLD_GLOBAL 0x100
```

The codegen produces a `call dlopen` / `call dlsym` with arguments
loaded into `%rdi` and `%rsi` per the System V ABI. The
function-pointer result from `dlsym` is left in `%rax` and can be
called indirectly (the codegen supports indirect `call *%rax` for
function-pointer variables — see `src/codegen.cpp:1343-1364`).

## What Works

- All of `dlopen` / `dlsym` / `dlclose` / `dlerror` when declared
  as `extern`.
- `void *` return type — the library handle is left in `%rax`
  and can be passed back to `dlsym` and `dlclose`.
- String arguments — the library path is loaded with
  `lea .Lstr_N(%rip), %rax` (`src/codegen.cpp:1534-1541`).
- Linking against `-ldl` is required at link time.

## Limitations

- No bundled `<dlfcn.h>` header. The user must declare the
  prototypes and the `RTLD_*` flag values.
- The compiler does not implement the dynamic loader; `libdl` is
  the one that actually maps the shared library into memory.
- Function-pointer types returned by `dlsym` are stored in `void *`
  locals; calling them requires either casting to a function-pointer
  type or storing them in a `void (*)(int)` variable (the latter
  works with the indirect-call path in `src/codegen.cpp:1352-1361`).

## Example

```c
void *dlopen(char *path, int flags);
void *dlsym(void *handle, char *symbol);

int main() {
    return 0;
}
```

The example does not actually load a library — it returns 0 from
`main` to exercise the prototype-parsing path. Tests in
`tests/test_dynamic_loading.cpp` cover the standard declarations.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| `parse_declaration` | `src/parser.cpp:300-336` | `extern` declarations produce a no-body `FunctionDeclNode` |
| Pointer-type parsing | `src/parser.cpp:255-262` | `void *` and `char *` parameters |
| `parse_function_decl` | `src/parser.cpp:578-615` | Forward decl path |
| Forward-decl emit | `src/codegen.cpp:305-309` | Skips emission when `body == nullptr` |
| Function call codegen | `src/codegen.cpp:1288-1364` | System V ABI register assignment |
| Indirect call | `src/codegen.cpp:1352-1361` | `call *%rax` for function-pointer variables |
