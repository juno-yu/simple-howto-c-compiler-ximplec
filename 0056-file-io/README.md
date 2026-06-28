# Lesson 0056: File I/O

## Status: 📋 Planned | Phase: Stdlib Tier B | Effort: Medium (8-12h)

## Objective

Implement FILE* operations and file descriptor I/O.

## Functions

| Function | Complexity |
|----------|------------|
| `open(path, flags)` | Easy |
| `close(fd)` | Trivial |
| `read(fd, buf, n)` | Easy |
| `write(fd, buf, n)` | Easy |
| `fopen(path, mode)` | Medium |
| `fclose(fp)` | Easy |
| `fread/fwrite` | Medium |
| `fgets/fputs` | Medium |
| `fscanf/fprintf` | Hard |

## Implementation Checklist

- [ ] Implement open/close/read/write via syscalls
- [ ] Implement FILE struct with buffer
- [ ] Implement fopen/fclose
- [ ] Implement fread/fwrite
- [ ] Implement fgets/fputs
- [ ] Test: read a file and print its contents
