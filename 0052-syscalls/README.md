# Lesson 0052: System Calls

## Status: 📋 Planned | Phase: Stdlib Tier A | Effort: Medium (4-6h)

## Objective

Implement Linux syscall interface for I/O and memory.

## Implementation Checklist

- [ ] `syscall` instruction wrapper
- [ ] `write(fd, buf, len)` → syscall 1
- [ ] `read(fd, buf, len)` → syscall 0
- [ ] `brk(addr)` → syscall 12 (for malloc)
- [ ] `exit(code)` → syscall 60
- [ ] Test: write "hello" to stdout via syscall
