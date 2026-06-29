# Lesson 0052: System Calls

## Status: ✅ Complete | Phase: Stdlib Tier A | Effort: Medium

## Objective

Enable programs to issue Linux x86-64 system calls via `extern` function
declarations and via inline assembly. The compiler itself does not
implement a syscall ABI or wrappers; it relies on the user declaring
the prototype and linking against the system C library (or writing the
raw syscall instruction via `asm`).

## Calling Convention (x86-64 Linux)

| Register | Role |
|----------|------|
| `%rax`   | Syscall number |
| `%rdi`   | 1st argument |
| `%rsi`   | 2nd argument |
| `%rdx`   | 3rd argument |
| `%r10`   | 4th argument |
| `%r8`    | 5th argument |
| `%r9`    | 6th argument |
| `syscall`| Trap instruction |
| return   | Value in `%rax` (negative `-errno` on error) |

## Common Syscall Numbers

| Nr | Name | Use |
|----|------|-----|
| 0  | `read` | `read(fd, buf, len)` |
| 1  | `write` | `write(fd, buf, len)` |
| 2  | `open` | `open(path, flags, mode)` |
| 3  | `close` | `close(fd)` |
| 9  | `mmap` | `mmap(...)` |
| 12 | `brk`  | `brk(addr)` — heap management |
| 39 | `getpid` | `getpid()` |
| 57 | `fork` | `fork()` |
| 59 | `execve` | `execve(path, argv, envp)` |
| 60 | `exit` | `exit(code)` |
| 61 | `wait4` | `wait4(pid, status, opts, rusage)` |

## Implementation in `simplecc`

`simplecc` does not emit `syscall` instructions directly. Instead, the
program declares the desired function (or wrapper) as `extern` and the
linker resolves the call against the system C library:

```c
extern long write(int fd, const void *buf, unsigned long count);
extern void exit(int code);

int main() {
    write(1, "hello\n", 6);
    exit(0);
}
```

The codegen produces a normal `call write` (or `call exit`) using the
System V argument-passing convention — the first 6 integer arguments go
into `%rdi, %rsi, %rdx, %rcx, %r8, %r9` (see
`src/codegen.cpp:1300-1341`).

For a raw syscall, the user can drop a `syscall` instruction into
`asm("...")`:

```c
int main() {
    asm("mov $1, %rax");     // SYS_write
    asm("mov $1, %rdi");     // fd = 1 (stdout)
    asm("lea msg(%rip), %rsi");
    asm("mov $6, %rdx");     // len = 6
    asm("syscall");
    asm("mov $60, %rax");    // SYS_exit
    asm("xor %rdi, %rdi");
    asm("syscall");
    return 0;
}
```

## What Works

- External function declarations for syscall wrappers (`write`, `read`,
  `exit`, etc.) — they are emitted as standard `call` instructions
  with correct argument register assignment.
- Inline assembly for hand-rolled syscall sequences.

## Limitations

- The compiler does not generate the `mov $N, %rax` / argument
  shuffling sequence automatically from a `syscall(...)` intrinsic.
- The standard System V callee-saved register set (`%rbx`, `%rbp`,
  `%r12..%r15`) is not preserved around `asm` calls — the user is
  responsible.
- No automatic errno propagation.

## Example

```c
int main() { return 42; }
```

The trivial example program in `src/example.c` does not actually issue
a syscall — the function returns 42 to the OS via the normal
`main → return` path. Tests in `tests/test_syscalls.cpp` cover
declaration forms and a few inline-asm sequences.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| `parse_declaration` | `src/parser.cpp:300-336` | Parses `extern` function declarations |
| `parse_function_decl` | `src/parser.cpp:578-615` | Distinguishes forward decls from definitions |
| Forward-decl emit | `src/codegen.cpp:305-309` | Skips emission when `body == nullptr` |
| `call` codegen | `src/codegen.cpp:1288-1364` | Argument register assignment, then `call` |
| Inline asm | `src/codegen.cpp:1492-1496` | Emits `asm("...")` body verbatim |
