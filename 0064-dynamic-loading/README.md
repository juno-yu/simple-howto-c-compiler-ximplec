# Lesson 0064: Dynamic Loading

## Status: 📋 Planned | Phase: Stdlib Tier C | Effort: Hard

## Objective

Load shared libraries at runtime.

## Functions

| Function | Complexity |
|----------|------------|
| `dlopen()` | Hard |
| `dlsym()` | Hard |
| `dlclose()` | Easy |
| `dlerror()` | Easy |

## Implementation Checklist

- [ ] Implement dlopen: parse ELF, load segments
- [ ] Implement dlsym: symbol lookup
- [ ] Implement dlclose: unload library
- [ ] Implement dlerror: error reporting
- [ ] Support RTLD_LAZY, RTLD_NOW
- [ ] Test: load libm, call sin()
