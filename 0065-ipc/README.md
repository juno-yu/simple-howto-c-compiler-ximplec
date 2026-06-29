# Lesson 0065: Inter-Process Communication

## Status: ✅ Complete | Phase: Stdlib Tier C | Effort: Hard

## Objective

Enable programs to use POSIX IPC primitives — pipes (`pipe`),
named pipes (`mkfifo`), shared memory (`shmget`/`shmat`/`shmdt`/
`shmctl`), and message queues (`msgget`/`msgsnd`/`msgrcv`/
`msgctl`) — by declaring the functions as `extern` and letting the
linker resolve the calls against the system C library.

## Implementation

`simplecc` does not ship a bundled `<unistd.h>` or `<sys/ipc.h>`
header — the user declares the prototypes they need:

```c
// User-declared prototypes
int   pipe(int pipefd[2]);
int   mkfifo(const char *pathname, int mode);
int   shmget(int key, int size, int shmflg);
void *shmat(int shmid, const void *shmaddr, int shmflg);
int   shmdt(const void *shmaddr);
int   shmctl(int shmid, int cmd, void *buf);
int   msgget(int key, int msgflg);
int   msgsnd(int msqid, const void *msgp, int msgsz, int msgflg);
int   msgrcv(int msqid, void *msgp, int msgsz, long msgtyp, int msgflg);
int   msgctl(int msqid, int cmd, void *buf);
```

The codegen produces a `call pipe` (etc.) with the arguments loaded
into `%rdi` (and additional registers, as needed) per the System V
ABI.

## What Works

- All of `pipe` / `mkfifo` / `shmget` / `shmat` / `shmdt` /
  `shmctl` / `msgget` / `msgsnd` / `msgrcv` / `msgctl` when
  declared as `extern`.
- Array parameters — `int pipefd[2]` is parsed by `parse_param`
  (`src/parser.cpp:949-972`) and decayed to `int *pipefd`. The
  call site passes the address of the array in `%rdi`.
- Array indexing — `pipefd[0]` and `pipefd[1]` are emitted as
  `mov %rax, 0(%rdi)` and `mov %rax, 4(%rdi)` by the codegen.
- `int fd[2]; pipe(fd);` — the `&fd[0]` (i.e. `fd` decayed) is
  passed in `%rdi` and the callee writes the two fds into
  `fd[0]` and `fd[1]`.

## Limitations

- No bundled IPC header. The user must declare the prototypes
  and the `key_t` / `pid_t` / `size_t` types.
- The compiler does not implement any IPC primitive in user
  space. Linking against the system C library is required.
- The `msgsnd` / `msgrcv` `struct msgbuf` layout is not bundled.

## Example

```c
int pipe(int pipefd[2]);

int main() {
    int fd[2];
    pipe(fd);
    return 0;
}
```

The example calls `pipe(fd)` to exercise the full
`extern`-declaration → `call` codegen path. The two file
descriptors end up in `fd[0]` (read end) and `fd[1]` (write end).
Tests in `tests/test_ipc.cpp` cover the standard declarations.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| `parse_declaration` | `src/parser.cpp:300-336` | `extern` declarations produce a no-body `FunctionDeclNode` |
| Array param decay | `src/parser.cpp:949-972` | `int pipefd[2]` becomes `int *pipefd` |
| `parse_function_decl` | `src/parser.cpp:578-615` | Forward decl path |
| Forward-decl emit | `src/codegen.cpp:305-309` | Skips emission when `body == nullptr` |
| Function call codegen | `src/codegen.cpp:1288-1364` | System V ABI register assignment |
| Array indexing codegen | `src/codegen.cpp:1367-1430` | Scaled-indexed `mov` for `fd[0]`/`fd[1]` |
