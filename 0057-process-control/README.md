# Lesson 0057: Process Control

## Status: ✅ Complete | Phase: Stdlib Tier B | Effort: Medium

## Objective

Enable programs to call the standard POSIX process-control functions
(`fork`, `execve`, `execvp`, `waitpid`, `getpid`, `exit`) by declaring
them as `extern` and letting the linker resolve the calls against the
system C library.

## Implementation

`simplecc` does not ship a bundled header for the process-control
functions — the user declares the prototypes they need. The typical
set is:

```c
// User-declared prototypes
int   fork(void);
int   execve(const char *pathname, char *const argv[], char *const envp[]);
int   execvp(const char *file, char *const argv[]);
int   waitpid(int pid, int *status, int options);
int   getpid(void);
void  exit(int status);
```

The codegen produces a `call <name>` for each declaration, with
arguments passed in `%rdi..%r9` per the System V ABI. Pointer
parameters (`char **argv`, `int *status`) are loaded into the
appropriate register with `lea <label>(%rip), %rax` for string
literals or `lea <offset>(%rbp), %rax` for the address of a local.

## What Works

- All of `fork` / `execve` / `execvp` / `waitpid` / `getpid` / `exit`
  when declared as `extern`.
- `int **argv` (pointer-to-pointer) parameters — the compiler's
  pointer-type parsing (`src/parser.cpp:255-262`) chains `*` to the
  base type, producing `char **` for `char *argv[]` parameters.
- Zero-argument function calls — `fork()` is emitted as a plain
  `call fork` with no preceding argument moves.
- The return value (e.g. the child PID from `fork`) is left in `%rax`
  and can be used in subsequent expressions.

## Limitations

- The bundled `lib/stdlib.h` does not include process-control
  functions. The user must declare them.
- The compiler does not generate any fork-safety wrappers (no
  setjmp/longjmp, no async-signal-safe annotations).
- `vfork` and `posix_spawn` are not bundled; the user can declare
  them if needed.

## Example

```c
int fork(void);
int main() { return 0; }
```

The example in `src/example.c` declares `fork` but does not call it
— it returns 0 from `main` to exercise the prototype-parsing path.
Tests in `tests/test_process_control.cpp` cover the standard
declarations and a few simple calls.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| `parse_declaration` | `src/parser.cpp:300-336` | `extern` declarations produce a no-body `FunctionDeclNode` |
| Pointer-type parsing | `src/parser.cpp:255-262` | `char **` is built by chaining two `*` tokens |
| Forward-decl emit | `src/codegen.cpp:305-309` | Skips emission when `body == nullptr` |
| Function call codegen | `src/codegen.cpp:1288-1364` | System V ABI register assignment |
| Address-of codegen | `src/codegen.cpp:1466-1480` | `lea <offset>(%rbp), %rax` for `&status` |
