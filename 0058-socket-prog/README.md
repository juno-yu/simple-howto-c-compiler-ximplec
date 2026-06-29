# Lesson 0058: Socket Programming

## Status: ✅ Complete | Phase: Stdlib Tier B | Effort: Medium

## Objective

Enable programs to use the POSIX socket API (`socket`, `bind`,
`listen`, `accept`, `connect`, `send`, `recv`, etc.) by declaring the
functions as `extern` and letting the linker resolve the calls against
the system C library (which provides the actual `syscall`-based
implementations).

## Implementation

`simplecc` does not ship a bundled `<sys/socket.h>` header — the user
declares the prototypes they need. The typical set is:

```c
// User-declared prototypes
int  socket(int domain, int type, int protocol);
int  bind(int sockfd, const void *addr, int addrlen);
int  listen(int sockfd, int backlog);
int  accept(int sockfd, void *addr, int *addrlen);
int  connect(int sockfd, const void *addr, int addrlen);
long send(int sockfd, const void *buf, long len, int flags);
long recv(int sockfd, void *buf, long len, int flags);
int  close(int fd);
```

The codegen produces a `call <name>` for each declaration, with
arguments passed in `%rdi..%r9` per the System V ABI. The `void *addr`
parameter is loaded into `%rsi` with `lea <label>(%rip), %rax`.

## What Works

- All of `socket` / `bind` / `listen` / `accept` / `connect` /
  `send` / `recv` when declared as `extern`.
- Three- and four-argument calls — the call site loads each argument
  into the next register in the sequence (`%rdi`, `%rsi`, `%rdx`,
  `%rcx`).
- Returning the socket file descriptor in `%rax` (the standard
  return-value register).
- `int *addrlen` output parameter — the caller passes the address of
  a local `int` using the `&` operator, and `accept` writes the
  address length into it.

## Limitations

- The bundled library does not declare any of the socket functions.
  The user must provide the prototypes and the `struct sockaddr_in`
  definition.
- No struct-layout support for `sockaddr_in` beyond the basic
  `struct { ... }` syntax. The user must layout the address struct
  carefully (the compiler lays out fields in declaration order with
  no padding — see `src/semantic.cpp` for the struct-layout pass).
- No `select` / `poll` / `epoll` declarations are bundled.

## Example

```c
int socket(int domain, int type, int protocol);
int main() { return 0; }
```

The example declares the basic `socket` function and returns 0
without calling it — it exercises the prototype-parsing path.
Tests in `tests/test_socket_prog.cpp` cover the standard
declarations.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| `parse_declaration` | `src/parser.cpp:300-336` | `extern` declarations produce a no-body `FunctionDeclNode` |
| `parse_function_decl` | `src/parser.cpp:578-615` | Forward decl path sets `body = nullptr` |
| Pointer-type parsing | `src/parser.cpp:255-262` | `void *` and `int *` parameters |
| Forward-decl emit | `src/codegen.cpp:305-309` | Skips emission when `body == nullptr` |
| Function call codegen | `src/codegen.cpp:1288-1364` | System V ABI register assignment |
| Address-of codegen | `src/codegen.cpp:1466-1480` | `lea <offset>(%rbp), %rax` for `&addrlen` |
