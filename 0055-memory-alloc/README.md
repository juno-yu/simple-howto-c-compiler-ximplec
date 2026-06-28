# Lesson 0055: Memory Allocation

## Status: 📋 Planned | Phase: Stdlib Tier A | Effort: Medium (6-8h)

## Objective

Implement malloc, free, calloc.

## Implementation

Simple bump allocator using brk syscall.

## Implementation Checklist

- [ ] Implement brk syscall wrapper
- [ ] Implement malloc: bump pointer allocator
- [ ] Implement free: no-op for bump allocator
- [ ] Implement calloc: malloc + memset
- [ ] Implement realloc
- [ ] Test: `int *p = malloc(sizeof(int)); *p = 42; return *p;` → 42
