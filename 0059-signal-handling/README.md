# Lesson 0059: Signal Handling

## Status: ✅ Complete | Phase: Stdlib Tier B | Effort: Medium

## Objective

Enable programs to install and dispatch signal handlers using the
standard POSIX signal functions (`signal`, `sigaction`, `kill`,
`raise`) by declaring the functions as `extern` and letting the
linker resolve the calls against the system C library.

## Implementation

`simplecc` does not ship a bundled `<signal.h>` header — the user
declares the prototypes they need. The typical set is:

```c
// User-declared prototypes
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
int          sigaction(int signum, const void *act, void *oldact);
int          kill(int pid, int signum);
int          raise(int signum);
```

The codegen produces a `call <name>` for each declaration. Function
pointer parameters (`sighandler_t handler`) are loaded into the
appropriate register as 8-byte pointer values.

## What Works

- All of `signal` / `sigaction` / `kill` / `raise` when declared as
  `extern`.
- Function-pointer types — `void (*handler)(int)` is parsed by
  `parse_function_pointer` in the declaration path
  (`src/parser.cpp:527-547`) and stored as a type with `(*)`
  in the middle.
- The `signal` function is one of the few standard C functions whose
  return type is a function pointer — the codegen leaves the result
  in `%rax` as a normal pointer.
- Multi-argument calls (`sigaction(signum, act, oldact)` lowers to
  three register moves followed by `call sigaction`).

## Limitations

- No bundled `<signal.h>` header. The user must declare the
  prototypes and the signal numbers (`SIGINT`, `SIGTERM`, etc.).
- No `sigset_t` / `sigemptyset` / `sigaddset` support beyond what
  the user can hand-roll.
- The function-pointer return type is recognised but the AST does
  not enforce a function-pointer type for the first argument to
  `signal`.

## Example

```c
int main() {
    int x = 42;
    return x;
}
```

The example does not actually use any signal functions — it returns
42 from `main` to exercise the basic compilation path. Tests in
`tests/test_signal_handling.cpp` cover the standard declarations.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| `parse_declaration` | `src/parser.cpp:300-336` | `extern` declarations produce a no-body `FunctionDeclNode` |
| Function pointer types | `src/parser.cpp:527-547` | `void (*)(int)` parameter parsing |
| `parse_function_decl` | `src/parser.cpp:578-615` | Forward decl path |
| Forward-decl emit | `src/codegen.cpp:305-309` | Skips emission when `body == nullptr` |
| Function call codegen | `src/codegen.cpp:1288-1364` | System V ABI register assignment |
